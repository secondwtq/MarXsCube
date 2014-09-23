_binded_attr = { }

_object = { }

_object.new = function (self, t)
	r = t or { }
	setmetatable(r, self)
	self.__index = self
	if (self.__init__) then self.__init__(r) end
	return r
end

object = _object

_binded_attr.bindedattr_cont = object:new({

	initial_value = 0,

	_controllers = { },

	get = function (self)
		local ret = self.initial_value
		for i, v in self._controllers do
			ret = v(ret)
		end
		return ret
	end,

	add_controller = function(self, controller)
		table.insert(self._controllers, controller)
		return controller
	end,

	set = function(self, value)
		self.initial_value = value
	end,

})


_binded_attr.binded_attr = object:new({

	attr_controller = object:new({
		controller = function(self, value) return value end,
		priority = 0,
		compare_to = { },
	}),

	initial_controller = function (self) return 0 end,

	__init__ = function (self)
		self._controllers = { }
	end,

	_controllers = { },

	get = function(self)
		local ret = self:initial_controller()
		for i, v in ipairs(self._controllers) do
			ret = v.controller(self, ret)
		end
		return ret
	end,

	add_controller = function(self, controller, priority)
		local c = self.attr_controller:new({
			parent = self,
			controller = controller,
			priority = priority,
		})
		-- table.insert(self._controllers, controller)
		table.insert(self._controllers, c)
		table.sort(self._controllers, function(a, b) return (a.priority > b.priority) end)
		return controller
	end,

	set_initial = function(self, controller)
		self.initial_controller = controller
	end,

})

-- return _binded_attr