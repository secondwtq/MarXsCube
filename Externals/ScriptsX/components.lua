-- this file is part of Project MarXsCube Prototype.

-- subproject:
--
--		Lunar?
--		ScriptX?
--
-- basic support of components
--
-- including:
--
--		* components_container
--		* component
--		* subcomponent
--		* cosubcomponent
--
-- dependency:
--
--		* lobject
--		* Lua coroutine
--
--	use in other modules:
--
--		components = require 'components'
--
--	I may consider other name for components library in the future
--
--	changelog:
--
--		* 2014.9.29 EVE initial commit.
--		* 2014.10.4 added getdatatable function for comp. and subcomp.
--
--	http://github.com/secondwtq/MarXsCube

_module_components = { }

local lobject = require 'object'

_module_components.components_container = lobject.object:new({

	parent = nil,

	__components = { },

	data_global = { },
	data = { },

	__init__ = function (self)
		-- print("initing components_container", self)
		self.__components = { }
		self.data = { }
		self.data_global = { }
	end,

	init = function(self, parent)
		self.parent = parent
	end,

	add_component = function (self, component)
		local comp = component:new()

		table.insert(self.__components, comp)
		self.data[component.name] = { }
		comp:_on_create(self)

		return comp
	end,

	update = function (self)
		for i, v in ipairs(self.__components) do
			v:_on_update()
		end
	end,

})

_module_components.component = lobject.object:new({

	parent = nil,

	name = "DEFAULT_COMPNODE",

	subcomponents = { },

	__subcomponents = { },

	_on_create = function (self, parent)
		self.parent = parent

		local _n_subcomp = { }

		for i, v in ipairs(self.__subcomponents) do
			table.insert(_n_subcomp, v:new())
		end

		self.__subcomponents = _n_subcomp

		for i, v in ipairs(self.__subcomponents) do
			v:_on_create(self)
		end

		self:on_create(parent)
	end,

	_on_update = function (self)
		for i, v in ipairs(self.__subcomponents) do
			v:_on_update()
		end

		self:on_update();
	end,

	on_create = function (self, parent)

	end,

	on_update = function (self)

	end,

	__init__ = function (self)
		print("initing component", self)
		self.__subcomponents = { }

		for i, v in ipairs(self.subcomponents) do
			table.insert(self.__subcomponents, v)
		end
	end,

	set_datafield = function(self, fieldname, value)
		self.parent.data[self.name][fieldname] = value
	end,

	get_datafield = function (self, fieldname)
		return self.parent.data[self.name][fieldname]
	end,

	get_datatable = function (self)
		return self.parent.data[self.name]
	end,

	get_container = function (self)
		return self.parent
	end,

	container_parent = function (self)
		return self:get_container().parent
	end,

})

_module_components.subcomponent = lobject.object:new({

	parent = nil,

	name = "INITIAL_SUBCOMPNODE",

	_on_create = function (self, parent)
		self.parent = parent
		self:on_create(parent)
	end,

	_on_update = function (self)
		self:on_update()
	end,

	on_create = function(self, parent)

	end,

	on_update = function(self)

	end,

	get_container = function (self)
		return self.parent:get_container()
	end,

	container_parent = function (self)
		return self.parent:container_parent()
	end,

	set_datafield = function(self, fieldname, value)
		self.parent:set_datafield(fieldname, value)
	end,

	get_datafield = function (self, fieldname)
		return self.parent:get_datafield(fieldname)
	end,

	get_datatable = function (self)
		return self.parent:get_datatable()
	end,

})

_module_components.cosubcomponent = _module_components.subcomponent:new({

	__coroutine = nil,

	_on_create = function (self, parent)
		_module_components.subcomponent._on_create(self, parent)

		self.__coroutine = coroutine.create(self.co_execute)

		self:on_cocreate(parent)
	end,

	_on_update = function (self)
		_module_components.subcomponent._on_update(self)

		coroutine.resume(self.__coroutine, self)
	end,

	on_update = function (self)

	end,

	on_create = function (self, parent)

	end,

	on_cocreate = function (self, parent)

	end,

	on_coupdate = function (self)

	end,

})

return _module_components
