--package.path = package.path .. ';../ScriptsX/?.lua'

local components = require 'components'
local bindedattr = require 'binded_attr'

local __composer = { }

local subcomp_TechnoColorMultiply = components.subcomponent:new({

	name = "TechnoColorMultiply_Initial",

	-- use set_datafield and get_datafield for general Techno data operation
	--   of the component itself
	-- the two functions can be used after both Components and Sub Components

	-- use self:get_container() to get the Components Container
	-- use self:container_parent() to get the parent of Components Container
	-- the two functions can be used after both Components and Sub Components

	-- Techno data is stored in the data table of Components Container
	--	and all Sub Components of certain Component share a same space of Techno data.
	--		just like : container.data[<name of components>][<fieldname>]

	-- current I have no idea of how to share data between different Components of
	--	the same Component Container
	-- but there is a 'data_global' field reserved in the Component Container
	--  may be used to store references of Components


	-- coroutines and bound attributes is encouraged to be used with Components.
	on_create = function (self, parent)

		-- * BOUND ATTRIBUTE *
		-- bound attribute, introduced in bindedattr module of ScriptX?
		-- I don't know whether it can do exactly yet, but it is really an amazing idea
		-- generally we use bindedattr.binded_attr

		-- it has an initial value, which is actually a function, use set_initial() to specific it.

		-- and then add controller function with add_controller(), controllers should be added at the
		-- beginning, and its function signature should be <TRANSFORMED_VALUE> foo(self, <ORIGINAL_VALUE>)

		-- * GETTER & SETTER *
		-- ttter module in ScriptX? allows accessing bound attribute as common attribute
		-- comp/subcomp:get_datatable() function is added for it
		-- use bindedattr.add_bound_attr(datatable, attrname) to add a binded attr with getter

		-- the bound attribute object itself, which add_bound_attr returns,
		--	is in fact stored in _<Key>_ field of datatable
		-- a bindedattr.get_bound_attr() function is also provided to fetch it later

		-- it also allows a direct assignment of initial value

		local datatable = self:get_datatable()

		local attr_ColorMultiply = bindedattr.add_bound_attr(datatable, 'ColorMultiplyAttr')

		datatable.ColorMultiplyAttr = { 1.0, 1.0, 1.0, 1.0 }

		attr_ColorMultiply:add_controller(function (self_, value)
			local multiply = 1.0
			if ModEnvironment.CurMouseOn ~= nil and self:container_parent().RTTIID == ModEnvironment.CurMouseOn.RTTIID then
				multiply = 1.6
			end
			return {value[1]*multiply, value[2]*multiply, value[3]*multiply, value[4]}
		end, 0)

		attr_ColorMultiply:add_controller(function (self_, value)
			local multiply = 1.0
			if self:container_parent().isDisabled == true then
				if self:container_parent().disabledTimer.Enabled == false then self:container_parent().disabledTimer:SwitchStart() end
				local d = math.abs(self:container_parent().disabledTimer:GetPercentage()-50) / 100.0
				multiply = 0.5 + 0.5 * d
				self:container_parent().disabledTimer:Update()
			end
			return {value[1]*multiply, value[2]*multiply, value[3]*multiply, value[4]}
		end, 0)
	end,

	on_update = function (self)
		self:get_container().parent.bodyElement.colorMultiply = Utility.Homogeneous4D(unpack(self:get_datafield('ColorMultiplyAttr')))
	end,

})

-- * CUSTOM EVENTS *

-- * HOW TO DEFINE COMPONENTS *
-- ObjectTable(or something) -> Components Container -> Components -> Sub Components
-- Sub Components should be declared as below
-- you need to specific a name for both Components and Sub Components
-- you are also responsible to set an alias, so other code can access it through comp_cont.a[<alias>]

-- now the table name of Components use a prefix 'comp_', and Sub Components 'subcomp_'
local comp_TechnoColorMultiply = components.component:new({
	name = "TechnoColorMultiply",
	alias = "TechnoColorMultiply",

	subcomponents = {
		subcomp_TechnoColorMultiply
	}
})

__composer.comp_TechnoColorMultiply = comp_TechnoColorMultiply

return __composer
