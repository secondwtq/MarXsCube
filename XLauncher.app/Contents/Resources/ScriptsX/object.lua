object = { }

_object = { }

_object.new = function (self, t)
	r = t or { }
	setmetatable(r, self)
	self.__index = self
	if self.__init__ ~= nil then self:__init__() end
	return r
end

object.object = _object

return object