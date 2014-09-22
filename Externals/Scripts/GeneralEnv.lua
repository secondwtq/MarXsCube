ModEnvironment = { }

Enums.ModEnv = { }

Enums.ModEnv.GenStates = {
	Idle = 0,
	Selected_Single = 1,
}

Enums.ModEnv.Mission = {
	unknown = -1,
	Idle = 0,
	Moving = 1
}

Enums.ModEnv.MovingState = {
	unknown = -1,
	Moving = 0,
	Roating = 1,
	MovingRoating = 2
}

util = { }

function util.getObjectTable(obj)
	return Environment.ObjectTable[obj.RTTIID]
end

function util.getDegreeFromOffset(offX, offY)
	local offSq = math.sqrt((offX*offX)+(offY*offY))
	local ret = math.acos(offX/offSq)
	if offY > 0 then ret = 2*math.pi - ret end
	return ret
end

function util.roundDegree(deg)
	if deg > 360 then deg = deg - 360
	elseif deg < 0 then deg = deg + 360 end
	return deg
end

function util.roundRad(rad)
	if rad > 2*math.pi then rad = rad - 2*math.pi
	elseif rad < 0 then rad = rad + 2*math.pi end
	return rad
end

function util.boolToSign(src)
	if src then return 1
	else return -1 end
end

function util.intDiv(a, b)
	local _a, _b = math.modf(a/b)
	return _a
end

function util.getObjectCell(obj)
	return Objects.Map.GetInstance():GetCellAt(Objects.Map.CellFromCoord(obj:GetCoord()))
end

util.tblinsert = table.insert

ModEnvironment.CurGenState = Idle
ModEnvironment.SelectedUnit_Single = nil

ModEnvironment.CurMouseOn = nil

ModEnvironment.Functions = { }

function ModEnvironment.Functions.applyKeyboardCommandTo(self, keycode)
	if keycode == Enums.Key.S then
		if self:WhatAmI() == Enums.RTTITypeID.Techno then
			local _self = Utility.toTechno(self)
			if _self:onGround() then _self.Physics:setVelocity(0) end
			util.getObjectTable(_self).Mission = Enums.ModEnv.Mission.Idle
		end
	end
	if keycode == Enums.Key.D then
		if self:WhatAmI() == Enums.RTTITypeID.Techno then
			local _self = Utility.toTechno(self)
			-- _self.Physics:applyCentralImpulse_Directional(-1000)
			util.getObjectTable(_self).MovingState_Reverse = true
		end
	end
end

function ModEnvironment.Functions.moveTechno(self, target_coord)
	local src_cell = Objects.Map.GetInstance():GetCellAt(Objects.Map.CellFromCoord(self:GetCoord()))
	local cell = Objects.Map.GetInstance():GetCellAt(Objects.Map.CellFromCoord(target_coord))
	if self:getTechnoType().ScriptType.physics.locomotor_type == "default" then
		-- local startT = Utility.GetTime()
		local succeeded = Utility.Pathfinding_Find(self, src_cell, cell, self:getPathfindingcache())
		if succeeded == true then
			self:getPathfindingcache():inc()
			self:setDestnation(target_coord)
			util.getObjectTable(self).Mission = Enums.ModEnv.Mission.Moving
			util.getObjectTable(self).MovingState = Enums.ModEnv.MovingState.Moving
			self.Physics:activate()
			self.Physics:applyCentralForce_Directional(self:getTechnoType().ScriptType.physics.locomotor_args.engineforce)
		end
		-- print(Utility.GetTime() - startT)
	end
end

function ModEnvironment.Functions.createTechno(scriptType, coord, phy)
	phy = (phy==nil and false or phy)
	local initialType = scriptType:InitialType()
	local tp = initialType:WhatAmI()
	local obj
	if tp == Enums.RTTITypeID.TechnoType then
		obj = Objects.Type_Techno.createTechno(initialType)
	end
	if obj ~= nil then
		obj.EnablePhysics = phy;
		obj:SpawnAtMapCoord(coord)
		return obj
	end
	return nil
end

function ModEnvironment.Functions.createAnim(scriptType, coord)
	local initialType = scriptType:InitialType()
	local tp = initialType:WhatAmI()
	local obj
	if tp == Enums.RTTITypeID.AnimType then
		obj = Objects.Type_Anim.createAnim(initialType)
	end
	if obj ~= nil then
		obj:SpawnAtMapCoord(coord)
		return obj
	end
	return nil
end