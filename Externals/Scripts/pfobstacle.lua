local components = require 'components'
local H = require 'Helpers'

local comp_PathfindingObstacle = components.component:new {
	name = "PathfindingObstacle",
	alias = "PathfindingObstacle"
}

function comp_PathfindingObstacle:on_spawn()

	local args = H.scriptType_TechnoRTTITable(self:container_parent()):property 'pathfinding_obstacle'
	if args then
		if args.enabled then
			local core = H.Techno_TechnoRTTIIDTable(self:container_parent())
			local initial_pos = core:GetCoord()
			local offset = args.offset

			if args.static then
				if args.shape == "RECT" then

					local p0 = Utility.CubePoint(initial_pos.x-args.width/2+offset[1], initial_pos.y-args.height/2+offset[2])
					local p1 = Utility.CubePoint(initial_pos.x-args.width/2+offset[1], initial_pos.y+args.height/2+offset[2])
					local p2 = Utility.CubePoint(initial_pos.x+args.width/2+offset[1], initial_pos.y+args.height/2+offset[2])
					local p3 = Utility.CubePoint(initial_pos.x+args.width/2+offset[1], initial_pos.y-args.height/2+offset[2])

					print('generating obstacle')
					print(p0.x, p0.y)
					print(p1.x, p1.y)
					print(p2.x, p2.y)
					print(p3.x, p3.y)

					local o1 = Grit.GritObstacle.create()
					o1:pts():push_back(p0)
					o1:pts():push_back(p1)
					o1:pts():push_back(p3)
					Grit.instance():add_obs(o1)

					local o2 = Grit.GritObstacle.create()
					o2:pts():push_back(p1)
					o2:pts():push_back(p2)
					o2:pts():push_back(p3)
					Grit.instance():add_obs(o2)

				end
			end

		end
	end
end

return { comp_PathfindingObstacle = comp_PathfindingObstacle }