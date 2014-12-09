
function Functions.Main_GameUpdateBegin()
	-- move camera with mouse
	local pos = Objects.Session.GetInstance().MousePosData.pos
	local sesInst = Objects.Session.GetInstance()
	local step = Enums.General.CameraMouseSlideStep
	local dir = Enums.Direction

	if (pos.x <= 12) then sesInst:CameraMove(dir.SouthWest, step) end
	if (1024 - pos.x <= 12) then sesInst:CameraMove(dir.NorthEast, step) end
	if (pos.y <= 12) then sesInst:CameraMove(dir.NorthWest, step) end
	if (768 - pos.y <= 12) then sesInst:CameraMove(dir.SouthEast, step) end
end

function Functions.Session_KeyPress(keyevent)
	-- ESC, then close the game
	if (keyevent.code == Enums.Key.Esc) then Utility.closeSession() end

	local sesInst = Objects.Session.GetInstance()
	local step = Enums.General.CameraKeyMoveStep
	local dir = Enums.Direction

	-- arrow key, move camera
	if (keyevent.code == Enums.Key.Left) then sesInst:CameraMove(dir.SouthWest, step) end
	if (keyevent.code == Enums.Key.Right) then sesInst:CameraMove(dir.NorthEast, step) end
	if (keyevent.code == Enums.Key.Up) then sesInst:CameraMove(dir.NorthWest, step) end
	if (keyevent.code == Enums.Key.Down) then sesInst:CameraMove(dir.SouthEast, step) end

	if ModEnvironment.CurGenState == Enums.ModEnv.GenStates.Selected_Single then
		ModEnvironment.Functions.applyKeyboardCommandTo(ModEnvironment.SelectedUnit_Single, keyevent.code)
	end
end

function Functions.Session_MousePress(mouseStatus)
	local TestAnimType = OBJECTS.TESTDDS:InitialType()
	-- local Anim = Objects.Type_Anim.createAnim(TestAnimType)
	local coord = Utility.GetCoordFromPoint(mouseStatus.pos)
	local cells = Objects.Map.CellFromCoord(coord)
	local cell = Objects.Map.GetInstance():GetCellAt(cells)
	-- Anim:SpawnAtMapCoord(coord)

	if mouseStatus.left == true then
		local x_ = function (ptr) return true end
		local ray = Physics.RayTestSingle.createRayTestSingle(coord, Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000))
		ray:perform()
		if (ray:hit()) then
			local selected = ray:getFirstObject().attachedToObject
			if selected:WhatAmI() == 7 and Utility.toTechno(selected):getTechnoType().ScriptType.clickselectable == true then
				ModEnvironment.CurGenState = Enums.ModEnv.GenStates.Selected_Single
				ModEnvironment.SelectedUnit_Single = selected
			end
		else
			if ModEnvironment.CurGenState == Enums.ModEnv.GenStates.Idle then

			elseif ModEnvironment.CurGenState == Enums.ModEnv.GenStates.Selected_Single then
				if ModEnvironment.SelectedUnit_Single:WhatAmI() == 7 then
					local selected = Utility.toTechno(ModEnvironment.SelectedUnit_Single)
					if selected:getTechnoType().ScriptType.clickmoveable == true then
						ModEnvironment.Functions.moveTechno(selected, coord)
					end
				end
			end
		end
	else 
		if ModEnvironment.CurGenState == Enums.ModEnv.GenStates.Selected_Single then
			ModEnvironment.CurGenState = Enums.ModEnv.GenStates.Idle
			ModEnvironment.SelectedUnit_Single = nil
		end
		print("Current cell location: ", cell.LocCell.x, cell.LocCell.y)
	end
end

function Functions.Session_MouseRelease(mouseEventData) end

function Functions.Session_MouseMove(mouseStatus)
	local coord = Utility.GetCoordFromPoint(mouseStatus.pos)
	local ray = Physics.RayTestSingle.createRayTestSingle(coord, Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000), function (ptr) return true end)
	ray:perform()
	if (ray:hit()) then
		local selected = ray:getFirstObject().attachedToObject
		if selected:WhatAmI() == 7 then
			ModEnvironment.CurMouseOn = Utility.toTechno(selected)
		end
	else ModEnvironment.CurMouseOn = nil end
end

function Functions.Abs_Anim_onCreate(creating, table) end

function Functions.Abs_Anim_onSpawn(self, table) end

function Functions.Abs_Anim_onUpdate(updating, table)
	-- anim.next
	if (updating:getAnimTimer().TimerEnded == true) then
		local next = updating:getAnimType().ScriptType.next
		if next ~= nil then
			local nextAnim = Objects.Type_Anim.createAnim(next:InitialType())
			nextAnim:SpawnAtMapCoord(updating.Location)
		end
	end
end

Import("Techno.lua")