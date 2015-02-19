local v = require 'Vhere'

local steering = { }

local C_min_speed = 50

function steering:steer_to_seek(target)
	local vel_desired = (target - self.parent:position()):nom()
	return vel_desired - self.parent:velocity()
end

