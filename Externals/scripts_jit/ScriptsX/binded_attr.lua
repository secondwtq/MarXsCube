_binded_attr = { }

local object = require 'object'

_binded_attr.bindedattr_cont = object.object:new({

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

_binded_attr.binded_attr = object.object:new({

	attr_controller = object.object:new({
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
			if v.enabled then
				ret = v.controller(self, ret)
			end
		end
		return ret
	end,

	add_controller = function(self, controller, priority)
		local c = self.attr_controller:new({
			parent = self,
			controller = controller,
			priority = priority,
			enabled = true,
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

local ttter = require 'ttter'

_binded_attr.add_bound_attr = function(table, key)

	local bound_actually_name = '_' .. key .. '_'

	table[bound_actually_name] = _binded_attr.binded_attr:new()

	ttter.property(table, key,
	
	function (table)
		return table[bound_actually_name]:get()
	end,
	function (table, value)
		table[bound_actually_name]:set_initial(function () return value end)
	end)

	return table[bound_actually_name]

end

_binded_attr.get_bound_attr = function(table, key)
	return table['_' .. key .. '_']
end

return _binded_attr
