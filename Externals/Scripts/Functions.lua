InputHandler = require 'InputHandler'
Helpers = require 'Helpers'

function Functions.Main_GameUpdateBegin()
	-- move camera with mouse
	local pos = Objects.Session.GetInstance().MousePosData.pos
	local sesInst = Objects.Session.GetInstance()
	local step = Enums.General.CameraMouseSlideStep
	local dir = Enums.Direction

	if (pos.x <= 24) then sesInst:CameraMove(dir.SouthWest, step) end
	if (1920 - pos.x <= 24) then sesInst:CameraMove(dir.NorthEast, step) end
	if (pos.y <= 24) then sesInst:CameraMove(dir.NorthWest, step) end
	if (1080 - pos.y <= 24) then sesInst:CameraMove(dir.SouthEast, step) end
end

function Functions.Session_KeyPress(keyevent)
	-- ESC, then close the game
	if (keyevent.code == Enums.Key.Esc) then
		Utility.closeSession()
	end

	local sesInst = Objects.Session.GetInstance()
	local step = Enums.General.CameraKeyMoveStep
	local dir = Enums.Direction

	-- arrow key, move camera
	if (keyevent.code == Enums.Key.Left) then sesInst:CameraMove(dir.SouthWest, step) end
	if (keyevent.code == Enums.Key.Right) then sesInst:CameraMove(dir.NorthEast, step) end
	if (keyevent.code == Enums.Key.Up) then sesInst:CameraMove(dir.NorthWest, step) end
	if (keyevent.code == Enums.Key.Down) then sesInst:CameraMove(dir.SouthEast, step) end

	InputHandler.KeyPress(keyevent.code)
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
		-- local rt_start = coord
		local rt_start = Utility.CoordStruct(coord.x-600, coord.y-600, coord.z-500)
		local rt_end = Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000)
		local ray = Physics.RayTestSingle.createRayTestSingle(rt_end, rt_start)
		ray:perform()
		if (ray:hit()) then
			-- delegate to InputHandler - MousePress OnObject
			InputHandler.MousePress_OnObject(mouseStatus, ray)
		else
			-- delegate to InputHandler - MousePress OnCell
			InputHandler.MousePress_OnCell(mouseStatus)
		end
	else
		InputHandler.MousePress_Right(mouseStatus)
		print("Current cell location: ", cell.LocCell.x, cell.LocCell.y, coord.x, coord.y)
	end
end

function Functions.Session_MouseRelease(mouseEventData) end

function Functions.Session_MouseMove(mouseStatus)
	local coord = Utility.GetCoordFromPoint(mouseStatus.pos)
	local rt_start = Utility.CoordStruct(coord.x-600, coord.y-600, coord.z-500)
	local ray = Physics.RayTestSingle.createRayTestSingle(rt_start, Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000), function (ptr) return true end)
	ray:perform()
	if (ray:hit()) then
		local selected = ray:getFirstObject().attachedToObject
		if selected:WhatAmI() == 7 then
			ModEnvironment.CurMouseOn = Utility.toTechno(selected)
		end
	else ModEnvironment.CurMouseOn = nil end

	InputHandler.MouseMove(mouseStatus)
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