components = require 'components'
bindedattr = require 'binded_attr'

composer = require 'composer'
Helpers = require 'Helpers'

function Functions.TechnoType_onLoad(self, table) end

function Functions.Abs_Techno_onUpdate(self, table)
	-- luatrace.tron { trace_file_name="mytrace" .. COUNT .. ".txt" }
	-- COUNT = COUNT + 1

	-- local scriptType = self:getTechnoType().ScriptType

	self.elements.direction = self.Direction.degree

	-- if scriptType.occupationtype == "local" then
	-- 	if table.occupationtype_local_curCell ~= nil then
	-- 		table.occupationtype_local_curCell:clearOccupyObject()
	-- 	end
	-- 	table.occupationtype_local_curCell = util.getObjectCell(self)
	-- 	table.occupationtype_local_curCell:setOccupyObject(self)
	-- end
	-- if scriptType.occupationtype == "centeredynamic" then
	-- 	for i, v in ipairs(table.occupationtype_centeredynamic_cells) do
	-- 		v:clearOccupyObject()
	-- 	end
	-- 	table.occupationtype_centeredynamic_cells = { }
	-- 	local curCell = util.getObjectCell(self)
	-- 	local x, y = scriptType.occupationfoundation_simple.x, scriptType.occupationfoundation_simple.y
	-- 	for i = curCell.LocCell.x-x, curCell.LocCell.x+x do
	-- 		for j = curCell.LocCell.y-y, curCell.LocCell.y+y do
	-- 			local cell = Objects.Map.GetInstance():GetCellAt(Utility.CellStruct(i, j))
	-- 			util.tblinsert(table.occupationtype_centeredynamic_cells, cell)
	-- 			cell:setOccupyObject(self)
	-- 		end
	-- 	end
	-- end

	table.components:update()

	-- luatrace.troff()
end

function Functions.Abs_Techno_onPhysicsTransformed(self, table)

end

function Functions.Abs_Techno_onSpawn(self, table)
	local scriptType = self:getTechnoType().ScriptType

	table.disabledTimer = Utility.CubeTimer(64, -1)

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

	table.components:spawn()
end

-- Components Container should be created in Object RTTIID table
--    it name should be 'table.components'
-- call the init(parent) method after allocing using the table(parent)
-- call init_components() method after all components are added to trigger on_init()

function Functions.Abs_Techno_onCreate(creating, table)
	print("Functions.Abs_Techno_onCreate: creating Techno ...")

	-- set general properties for RTTIID table, for helpers
	table.RTTIID = creating.RTTIID
	table.TechnoType = Helpers.TechnoType_Techno(creating)
	table.Type = table.TechnoType
	table.Techno = creating
	table.AttachedToObject = creating
	table.ScriptType = Helpers.scriptType_Techno(creating)

	table.GetCoord = function (self) return creating.GetCoord(creating) end

	function table:WhatAmI () return Enums.TechnoRTTIIDTable end

	local scriptType = Helpers.scriptType_Techno(creating)

	-- alloc, init comp cont, and add components to container
	table.components = components.components_container:new()
	table.components:init(table)

	for i, v in ipairs(scriptType.components) do
		table.components:add_component(v)
	end

	print("Functions.Abs_Techno_onCreate: initing components ...")
	table.components:init_components()

	table.isDisabled = false

	-- hack, same as table.typename, we need to add a name flag for TECHNOTYPE for debugging
	--   moreover, we need to get CubeCore Object from Object RTTIID table,
	--			and get Object RTTIID table from CubeCore Object
	--				get Type from both CubeCore Object and Object RTTIID table
	table.typename = scriptType.image

	creating.useCollSphere = scriptType.usecollsphere
	creating.rCollSphere = scriptType.rcollsphere

	print("Functions.Abs_Techno_onCreate: completed ...")
end
