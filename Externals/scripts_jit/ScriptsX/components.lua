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
--		* 2014.10.4 custom event support
--		* 2014.10.4 PM alias on comp.cont. & on_init function for comp. to access all comp.s
--		* 2014.10.4 EVE refined alias function call
--		* 2014.10.6 EVE fixed alias table instance bug
--
--	http://github.com/secondwtq/MarXsCube

_module_components = { }

local lobject = require 'object'

_module_components.components_container = lobject.object:new({

	parent = nil,

	__components = { },

	data_global = { },
	data = { },

	events = { },

	-- aliases
	a = { },

	__init__ = function (self)
		-- print("initing components_container", self)
		self.__components = { }
		self.data = { }
		self.data_global = { }
		self.a = { }

		self.events = { }
		local function container_event_dispatcher (table, key)
			return
				function ()
					for i, v in ipairs(self.__components) do
						v.events[key]()
					end
				end
		end
		setmetatable(self.events, { __index = container_event_dispatcher })

		-- custom component function call through aLIAS
		--  and the alias is not presented
		local a_notpresent = { }
		local a_notpresent_mt = { }
		function a_notpresent_mt.__index(table, value)
			return
				function () end
		end
		setmetatable(a_notpresent, a_notpresent_mt)

		local a_mt = { }
		function a_mt.__index(table, value)
			return a_notpresent
		end
		setmetatable(self.a, a_mt)
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

	init_components = nil,

	update = function (self)
		for i, v in ipairs(self.__components) do
			v:_on_update()
		end
	end,

	spawn = function (self)
		for i, v in ipairs(self.__components) do
			v:on_spawn()
		end
	end,

	phy_transform = function (self)
		for i, v in ipairs(self.__components) do
			v:on_phytrans()
		end
	end,

})

function _module_components.components_container:init_components()
	for i, v in ipairs(self.__components) do
		v:on_init()
	end
end

local events_container = {

	parent = nil,

	__index = nil,

	__newindex = nil,
}

function events_container.__index(table, value)
	return
		function ()
			local parent = rawget(table, 'parent')
		end
end

function events_container.__newindex(table, key, value)
	table['_' .. key .. '_'] = value
end

_module_components.component = lobject.object:new({

	parent = nil,

	name = "DEFAULT_COMPNODE",
	alias = "DEFAULT_COMPNODE_ALIAS",

	events = { },

	_on_create = function (self, parent)
		self.parent = parent

		parent.a[self.alias] = self
		
		self.data = self:get_datatable()

		self:on_create(parent)
	end,

	_on_update = function (self)
		self:on_update()
	end,

	on_create = function (self, parent)

	end,

	-- alloc compcont -> compcont:init
	-- compcont:add_component -> comp:_on_create -> comp:on_create -> compcont:init_components -> comp:on_init
	-- you can access alias in on_init, it is all for user
	on_init = function (self)

	end,

	on_phytrans = function (self)

	end,

	on_update = function (self)

	end,

	on_spawn = function (self)

	end,

	__init__ = function (self)
		self.events = { }
		setmetatable(self.events, events_container)
		rawset(self.events, "parent", self)
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
		return self.parent.parent
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
		return self.parent.parent
	end,

	container_parent = function (self)
		return self.parent.parent.parent
	end,

	set_datafield = function(self, fieldname, value)
		self.parent.parent.data[self.parent.name][fieldname] = value
	end,

	get_datafield = function (self, fieldname)
		return self.parent.parent.data[self.parent.name][fieldname]
	end,

	get_datatable = function (self)
		return self.parent.parent.data[self.parent.name]
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
