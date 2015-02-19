local components = require 'components'
local H = require 'Helpers'
local Placeholders = require 'Placeholders'
local object = require 'object'
local v = require 'Vhere'

local SteeringObstacle = object.object:new {
	radius = 32,
	pos = v.vector2d(0, 0)
}

comp_SteeringObstacle_global = {
	obstacles = { }
}

function comp_SteeringObstacle_global.point_scan(center, radius)
	local ret = { }
	for i, v in ipairs(comp_SteeringObstacle_global.obstacles) do
		local offset = v.pos - center
		if offset:len() - v.radius < radius then
			table.insert(ret, v)
		end
	end
	return ret
end

local comp_SteeringObstacle = components.component:new {
	name = 'SteerObstacle',
	alias = 'SteerObstacle'
}

function comp_SteeringObstacle:on_create()
	self.data.obstacle_obj = SteeringObstacle:new()
end

function comp_SteeringObstacle:on_spawn()
	local args = H.scriptType_TechnoRTTITable(self:container_parent()):property 'steering_obstacle'
	if args.enabled then
		self.data.obstacle_obj.radius = args.radius

		local position = v.coord2vec2(self:container_parent():GetCoord()) + v.tbl2vec2(args.offset)
		self.data.obstacle_obj.pos = position
		table.insert(comp_SteeringObstacle_global.obstacles, self.data.obstacle_obj)
	end
end

return { comp_SteeringObstacle = comp_SteeringObstacle }