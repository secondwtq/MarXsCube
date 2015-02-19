local v = require 'Vhere'

local steering = { }

function steering:steer_to_seek(target)
	local vel_desired = (target - self.parent:position()):nom()
	return vel_desired - self.parent:velocity()
end

