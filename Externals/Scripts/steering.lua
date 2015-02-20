local v = require 'Vhere'
local object = require 'object'

local steering = object.object:new {
	parent = nil,
	radius = 32,

	vel_desired = v.vector2d(0, 0)
}

local intersection = object.object:new {
	intersect = false,
	distance = 0,
	obstacle = nil
}

-- parent:
--		velocity() position() speed()

local C_min_speed = 50
local C_pred_time = 2.5
local C_est_time = 2
local C_dect_radius = 128

function steering:future_position_desired(time)
	return self.parent:position() + self.vel_desired * time
end

function steering:steer_to_seek(target)
	local vel_desired = target - self.parent:position()
	return vel_desired - self.parent:velocity()
end

function steering:steer_for_path(pathway)
	local speed = math.max(self.parent:speed(), C_min_speed)
	local offset = speed * C_pred_time

	local distance_now = pathway:mapPointToPathDistance(v.vec2glm3(self.parent:position()))
	local distance_target = distance_now + offset
	local pathpt_current, pathpt_target = v.coord2vec2(pathway:mapPathDistanceToPoint(distance_now)), 
										v.coord2vec2(pathway:mapPathDistanceToPoint(distance_target))

	return self:steer_to_seek(pathpt_target)
end

function steering:steer_to_avoid()
	local avoid = v.vector2d(0, 0)

	local obses = comp_SteeringObstacle_global.point_scan(self.parent:position(), C_dect_radius)
	if #obses == 0 then return avoid end
	local pos_future = self:future_position_desired(C_est_time)

	for i, obs in ipairs(obses) do
		local next = FindNextIntersectionWithSphere(self, pos_future, obs)
		local avoid_multi = 0.1

		if next.intersect then
			local time_to_coll = next.distance / self.parent:speed()
			avoid_multi = 2 * (C_est_time / time_to_coll);
		end

		local dir_opposite = self.parent:position() - obs.pos
		avoid = avoid + dir_opposite * avoid_multi
	end

	avoid = avoid / (#obses)
	local new_vel = self.vel_desired:reflect(avoid)
	-- print(string.format("%d, (%.2f, %.2f), (%.2f, %.2f)", #obses, new_vel.x, new_vel.y, self.vel_desired.x, self.vel_desired.y))
	return new_vel
end

function FindNextIntersectionWithSphere(vehicle, pos_future, obstacle)
	local curpos = vehicle.parent:position()

	local intersection = intersection:new({ obstacle = obstacle })

    local combinedRadius = vehicle.radius + obstacle.radius
    local movement = pos_future - curpos
    local direction = movement:nom()
    local vehicleToObstacle = obstacle.pos - curpos

    local proj_len = v.dot(direction, vehicleToObstacle)

    if proj_len > (movement:len() + combinedRadius) then
        return intersection
    end

    local projectedObstacleCenter = curpos + direction * proj_len;
    local obstacleDistanceToPath = (obstacle.pos - projectedObstacleCenter):len();

    if obstacleDistanceToPath > combinedRadius then
        return intersection
    end

    local halfChord = math.sqrt(combinedRadius*combinedRadius + obstacleDistanceToPath*obstacleDistanceToPath);

    if proj_len < 0 then
        if vehicleToObstacle:len() > combinedRadius then
            return intersection;
        end

        local intersectionPoint = projectedObstacleCenter - direction * halfChord;
        intersection.intersect = true;
        intersection.distance = (intersectionPoint - curpos):len();
        return intersection;
    end

    local intersectionPoint1 = projectedObstacleCenter - direction * halfChord;
    local intersectionPoint2 = projectedObstacleCenter + direction * halfChord;

    local intersectionPoint1Distance = (intersectionPoint1 - curpos):len();
    local intersectionPoint2Distance = (intersectionPoint2 - curpos):len();

    intersection.intersect = true;
    intersection.distance = math.min(intersectionPoint1Distance, intersectionPoint2Distance);

    return intersection;
end

return { steering = steering }