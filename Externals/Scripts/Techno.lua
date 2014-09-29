components = require 'components'

function Functions.TechnoType_onLoad(self, table)
	local pType = table
	local wImage = Objects.TextureManger.GetInstance():getTexture(table.image)
	table._image = wImage
	local sImage = Objects.TextureManger.GetInstance():getTexture(table.shadow)
	table._shadowimage = sImage
end

subcomp_TechnoColorMultiply = components.subcomponent:new({

	name = "TechnoColorMultiply_Initial",

	-- use set_datafield and get_datafield for general Techno data operation
	--   of the component itself
	-- the two functions can be used after both Components and Sub Components

	-- use self:get_container() to get the Components Container
	-- use self:container_parent() to get the parent of Components Container
	-- the two functions can be used after both Components and Sub Components

	-- Techno data is stored in the data table of Components Container
	--	and all Sub Components of certain Component share a same space of Techno data.
	--		just like : container.data[<name of components>][<fieldname>]

	-- current I have no idea of how to share data between different Components of
	--	the same Component Container
	-- but there is a 'data_global' field reserved in the Component Container
	--  may be used to store references of Components


	-- coroutines and bound attributes is encouraged to be used with Components.
	on_create = function (self, parent)
		self:set_datafield("ColorMultiply", bindedattr.binded_attr:new())

		self:get_datafield("ColorMultiply"):set_initial(function () return {1.0, 1.0, 1.0, 1.0} end)

		self:get_datafield("ColorMultiply"):add_controller(function (self_, value)
			local multiply = 1.0
			if ModEnvironment.CurMouseOn ~= nil and self:container_parent().RTTIID == ModEnvironment.CurMouseOn.RTTIID then
				multiply = 1.6
			end
			return {value[1]*multiply, value[2]*multiply, value[3]*multiply, value[4]}
		end, 0)

		self:get_datafield("ColorMultiply"):add_controller(function (self_, value)
			local multiply = 1.0
			if self:container_parent().isDisabled == true then
				if self:container_parent().disabledTimer.Enabled == false then self:container_parent().disabledTimer:SwitchStart() end
				local d = math.abs(self:container_parent().disabledTimer:GetPercentage()-50) / 100.0
				multiply = 0.5 + 0.5 * d
				self:container_parent().disabledTimer:Update()
			end
			return {value[1]*multiply, value[2]*multiply, value[3]*multiply, value[4]}
		end, 0)
	end,

	on_update = function (self)
		self:get_container().parent.bodyElement.colorMultiply = Utility.Homogeneous4D(unpack(self:get_datafield("ColorMultiply"):get()))
	end,

})

-- ObjectTable(or something) -> Components Container -> Components -> Sub Components
-- Sub Components should be declared as below
-- you need to specific a name for both Components and Sub Components

-- now the name of Components use a prefix 'comp_', and Sub Components 'subcomp_'
comp_TechnoColorMultiply = components.component:new({
	name = "TechnoColorMultiply",

	subcomponents = {
		subcomp_TechnoColorMultiply
	}
})

function Functions.Abs_Techno_onUpdate(self, table)
	local scriptType = self:getTechnoType().ScriptType

	self.elements:setDirection(self.Direction.degree)

	if table.Mission == Enums.ModEnv.Mission.Moving then
		if self.Physics:checkCollide_() then
			self:getRepathcache():insert(self:getPathfindingcache():getNex())
		end
		if self:getTechnoType().ScriptType.physics.locomotor_type == "default" then
			local args = self:getTechnoType().ScriptType.physics.locomotor_args
			if args.realtimeupdate == true then
				local dest_cell = Objects.Map.GetInstance():GetCellAt(Objects.Map.CellFromCoord(self.Destnation))
				local src_cell = Objects.Map.GetInstance():GetCellAt(Objects.Map.CellFromCoord(self:GetCoord()))
				if (not self.Physics:checkCollide_()) and (self:getRepathcache():size() > 3) then
					self:getRepathcache():resize(3)
					table.Mission = Enums.ModEnv.Mission.Moving
					table.MovingState = Enums.ModEnv.MovingState.Moving
				end
				local succeeded = Utility.Pathfinding_Find(self, src_cell, dest_cell, self:getPathfindingcache())
				if succeeded == true then self:getPathfindingcache():inc() end
			end
		end
	end

	if scriptType.occupationtype == "local" then
		if table.occupationtype_local_curCell ~= nil then
			table.occupationtype_local_curCell:clearOccupyObject()
		end
		table.occupationtype_local_curCell = util.getObjectCell(self)
		table.occupationtype_local_curCell:setOccupyObject(self)
	end
	if scriptType.occupationtype == "centeredynamic" then
		for i, v in ipairs(table.occupationtype_centeredynamic_cells) do
			v:clearOccupyObject()
		end
		table.occupationtype_centeredynamic_cells = { }
		local curCell = util.getObjectCell(self)
		local x, y = scriptType.occupationfoundation_simple.x, scriptType.occupationfoundation_simple.y
		for i = curCell.LocCell.x-x, curCell.LocCell.x+x do
			for j = curCell.LocCell.y-y, curCell.LocCell.y+y do
				local cell = Objects.Map.GetInstance():GetCellAt(Utility.CellStruct(i, j))
				util.tblinsert(table.occupationtype_centeredynamic_cells, cell)
				cell:setOccupyObject(self)
			end
		end
	end

	table.components:update()

	--table.bodyElement.colorMultiply = Utility.Homogeneous4D(unpack(table.bodyColorMultiply:get()))
end

function Locomotor_Default_setAngularVelocity(updating, args, curDeg, reqDeg, rev)
	if rev == -1 then
		Locomotor_Default_setAngularVelocity(updating, args, util.roundRad(curDeg+math.pi), util.roundRad(reqDeg+math.pi), 1)
		return
	end
	local getsign = function (x) return x/math.abs(x) end
	local _otherside = function(x) local r = x + math.pi if r >= 2*math.pi then r = r-2*math.pi end return r end
	local otherside = _otherside(curDeg)
	local minm = math.min(curDeg, otherside)
	local maxm = math.max(curDeg, otherside)
	local rotMult = 1
	if minm < reqDeg and reqDeg < maxm then rotMult = 1
	else rotMult = -1 end
	if getsign(math.sin(curDeg)) == -1 then rotMult = rotMult * -1 end
	updating.Physics:setMainRotationVelocity(args.rot * rotMult)
end

function Functions.Abs_Techno_onPhysicsTransformed(self, table)
	if table.Mission == Enums.ModEnv.Mission.Moving then
		if self:getTechnoType().ScriptType.physics.locomotor_type == "default" then
			local cache = self:getPathfindingcache()
			local coord = self:GetCoord()
			local cell = Objects.Map.GetInstance():GetCellAt(Objects.Map.CellFromCoord(coord))
			local args = self:getTechnoType().ScriptType.physics.locomotor_args
			if self:getPathfindingcache():inside(cell) then self:getPathfindingcache():jumpTo(cell) end
			if cache:ended() == true then
				self.Physics:setVelocity(0)
				self.Physics:setMainRotationVelocity(0)
				table.Mission = Enums.ModEnv.Mission.Idle
				table.MovingState_Reverse = false
				self:getRepathcache():clear()
				return
			end
			local nextCell = Utility.CoordStruct((cache:getCur():GetCoord().x+cache:getNex():GetCoord().x)/2,
											(cache:getCur():GetCoord().y+cache:getNex():GetCoord().y)/2,
											(cache:getCur():GetHeight()+cache:getNex():GetHeight())/2)
			-- local nextCell = Utility.CoordStruct(cache:getCur():GetCoord().x,
			-- 								cache:getCur():GetCoord().y,
			-- 								cache:getCur():GetHeight())
			-- local nextCell = Utility.CoordStruct((cache:getCur():GetCoord().x+cache:getNex():GetCoord().x+cache:getNexNex():GetCoord().x)/3,
			-- 								(cache:getCur():GetCoord().y+cache:getNex():GetCoord().y+cache:getNexNex():GetCoord().y)/3,
			-- 								(cache:getCur():GetHeight()+cache:getNex():GetHeight()+cache:getNexNex():GetHeight())/3)
			local offY, offX = nextCell.y-coord.y, nextCell.x-coord.x
			local curDeg_ = Utility.torad(self:getMainRotation())
			local reqDeg_ = util.getDegreeFromOffset(offX, offY)
			local _rev = util.boolToSign(table.MovingState_Reverse) * -1
			if table.MovingState_Reverse then reqDeg_ = util.roundRad(reqDeg_+math.pi) end

			if table.MovingState == Enums.ModEnv.MovingState.Moving then
				if math.abs(curDeg_ - reqDeg_) <= 0.36 then
					if self:onGround() == true then
						if args.realtimeupdate == true then self.Physics:applyCentralImpulse_Vertical(-self.Physics:getVerticalVelocity()*args.balance_force_mult) end
						if table.MovingState_Reverse == true and -self.Physics:getVelocity() < args.stablespeed then
							self.Physics:applyCentralForce_Directional(-args.engineforce)
						elseif self.Physics:getVelocity() < args.stablespeed and table.MovingState_Reverse == false then
							self.Physics:applyCentralForce_Directional(args.engineforce)
						end
						if math.abs(self.Physics:getVelocity()) >= args.stablespeed/2 then
							local curState = util.getDegreeFromOffset(self.Physics:getLinearVelocity().x, self.Physics:getLinearVelocity().y)
							if table.MovingState_Reverse == false then
								local deg = math.deg(curState)
								self.Physics:setDirectionTo(deg)
								self.Direction.degree = deg
							end
						end
					end
				else
					if args.realtimeupdate == false then self.Physics:setVelocity(0) end
					if self:onGround() == true then
						if args.realtimeupdate == true then self.Physics:applyCentralImpulse_Vertical(-self.Physics:getVerticalVelocity()*args.balance_force_mult) end
						Locomotor_Default_setAngularVelocity(self, args, curDeg_, reqDeg_, _rev)
						table.MovingState = Enums.ModEnv.MovingState.Roating
					end
				end
			end

			if table.MovingState == Enums.ModEnv.MovingState.Roating then
				if self:onGround() == true then
					if math.abs(self.Physics:getMainRotationVelocity()) < args.rot / 4 then
						Locomotor_Default_setAngularVelocity(self, args, curDeg_, reqDeg_, _rev)
					end
					if math.abs(curDeg_ - reqDeg_) < 0.24 then
						self.Physics:setMainRotationVelocity(0)
						table.MovingState = Enums.ModEnv.MovingState.Moving
					end
					self.Physics:applyCentralForce_Directional(-args.rot_negativeforce*_rev)
					if args.realtimeupdate == true then self.Physics:applyCentralImpulse_Vertical(-self.Physics:getVerticalVelocity()*args.balance_force_mult) end
				end
			end

		end
	end
end

function Functions.Abs_Techno_onSpawn(self, table)
	local scriptType = self:getTechnoType().ScriptType

	table.disabledTimer = Utility.CubeTimer(64, -1)

	table.Mission = Enums.ModEnv.Mission.Idle
	table.MovingState = Enums.ModEnv.MovingState.unknown
	table.MovingState_Reverse = false

	if scriptType.occupationtype == "local" then
		table.occupationtype_local_curCell = util.getObjectCell(self)
	end
	if scriptType.occupationtype == "buildingstatic" then
		local curCell = util.getObjectCell(self)
		local x, y = scriptType.occupationfoundation_simple.x, scriptType.occupationfoundation_simple.y
		for i = curCell.LocCell.x, curCell.LocCell.x+x do
			for j = curCell.LocCell.y, curCell.LocCell.y+y do
				Objects.Map.GetInstance():GetCellAt(Utility.CellStruct(i, j)):setOccupyObject(self)
			end
		end
	end
	if scriptType.occupationtype == "centeredynamic" then
		table.occupationtype_centeredynamic_cells = { }
		local curCell = util.getObjectCell(self)
		local x, y = scriptType.occupationfoundation_simple.x, scriptType.occupationfoundation_simple.y
		for i = curCell.LocCell.x-x, curCell.LocCell.x+x do
			for j = curCell.LocCell.y-y, curCell.LocCell.y+y do
				local cell = Objects.Map.GetInstance():GetCellAt(Utility.CellStruct(i, j))
				util.tblinsert(table.occupationtype_centeredynamic_cells, cell)
				cell:setOccupyObject(self)
			end
		end
	end

	if scriptType.physics.initialstatic == true then
		self.Physics:setToStatic() end
end

-- Components Container should be created in Object RTTIID table
--    it name should be 'table.components'
-- call the init(parent) method after allocing using the table(parent)

function Functions.Abs_Techno_onCreate(creating, table)
	-- a hack, it should be a helper function to get RTTIID from object table
	table.RTTIID = creating.RTTIID

	table.components = components.components_container:new()
	table.components:init(table)
	table.components:add_component(comp_TechnoColorMultiply)

	table.isDisabled = false
	local scriptType = creating:getTechnoType().ScriptType
	table.elements = { }

	-- hack, same as table.typename, we need to add a name flag for TECHNOTYPE for debugging
	--   moreover, we need to get CubeCore Object from Object RTTIID table,
	--			and get Object RTTIID table from CubeCore Object
	--				get Type from both CubeCore Object and Object RTTIID table
	table.typename = scriptType.image

	creating.useCollSphere = scriptType.usecollsphere
	creating.rCollSphere = scriptType.rcollsphere

	if scriptType.render_preset == "vehicle" then
		local bodyElement = Utility.RenderElement_DirectionedStatic.createElement(scriptType._image, scriptType.imagefaces)
		table.bodyElement = bodyElement
		local shadowElement = Utility.RenderElement_DirectionedStatic.createElement(scriptType._shadowimage, scriptType.shadowimagefaces)
		table.shadowElement = shadowElement
		shadowElement.colorMultiply = Utility.Homogeneous4D(0.1, 0.1, 0.1, 0.3)
		shadowElement.UseShadowProjection = true;

		creating.elements:insert(0, bodyElement)
		creating.elements:insert(-10, shadowElement)

		util.tblinsert(table.elements, bodyElement)
		util.tblinsert(table.elements, shadowElement)
	end
	if scriptType.render_preset == "structure" then
		local bodyElement = Utility.RenderElement_FramedStatic.createElement(scriptType._image)
		table.bodyElement = bodyElement

		local shadowElement = Utility.RenderElement_FramedStatic.createElement(scriptType._shadowimage)
		table.shadowElement = shadowElement
		shadowElement.colorMultiply = Utility.Homogeneous4D(0.1, 0.1, 0.1, 0.3)
		shadowElement.UseShadowProjection = true;

		creating.elements:insert(0, bodyElement)
		creating.elements:insert(-10, shadowElement)

		util.tblinsert(table.elements, bodyElement)
		util.tblinsert(table.elements, shadowElement)
	end

	table.Mission = Enums.ModEnv.Mission.unknown
	table.MovingState = Enums.ModEnv.MovingState.unknown

	table.bodyColorMultiply = bindedattr.binded_attr:new()

	table.bodyColorMultiply:set_initial(function () return {1.0, 1.0, 1.0, 1.0} end)
	table.bodyColorMultiply:add_controller(function (self, value)
		local multiply = 1.0
		if ModEnvironment.CurMouseOn ~= nil and creating.RTTIID == ModEnvironment.CurMouseOn.RTTIID then
			multiply = 1.6
		end
		return {value[1]*multiply, value[2]*multiply, value[3]*multiply, value[4]}
	end, 0)

	table.bodyColorMultiply:add_controller(function (self, value)
		local multiply = 1.0
		if table.isDisabled == true then
			if table.disabledTimer.Enabled == false then table.disabledTimer:SwitchStart() end
			local d = math.abs(table.disabledTimer:GetPercentage()-50) / 100.0
			multiply = 0.5 + 0.5 * d
			table.disabledTimer:Update()
		end
		return {value[1]*multiply, value[2]*multiply, value[3]*multiply, value[4]}
	end, 0)

end
