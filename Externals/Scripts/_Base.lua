config.bases.BaseBase = { }

function base_def_baseobject()

function config.bases.BaseBase:newObject(x)
	local ret = x or { }
	setmetatable(ret, self)
	self.__index = self
	return ret
end

function config.util.SetBase(object, base) setmetatable(object, { __index = base }) end

BASES.BaseObject = BASES.BaseBase:newObject({
	UID = -1,
	PTR = nil,
	InitialType = function(self) return self.PTR end,
	physics = {
		enabled = false,
		mass = 0.0,
		offset = { x = 0, y = 0, z = 0 },
		shape = {
			type = "BOX",
			offset = { x = 0, y = 0, z = 0 }
		}
	}
})

end

base_def_baseobject()