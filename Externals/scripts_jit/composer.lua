-- this file is part of Project MarXsCube Prototype.

-- subproject:
--
--		Lunar?
--
-- some basic components for test & example
--
-- including:
--
--		* comp_TechnoColorMultiply
--		* comp_RenderBasicBody
--		* comp_RenderElementsManger
--
-- dependency:
--
--		* components
--		* binded_attr
--		* Helpers
--		* Placeholders
--
--	use in other modules:
--
--		composers = require 'composers'

--
--	changelog:
--
--		* 2014.10.4 PM initial commit, added comp_TechnoColorMultiply.
--		* 2014.10.4 EVE updated.
--		* 2014.10.5 EVE refined comp_RenderBasicBody & comp_RenderElementsManger
--		* 2014.10.6 EVE compleing 3 comps
--
--	http://github.com/secondwtq/MarXsCube


package.path = package.path .. ';../ScriptsX/?.lua'

local components = require 'components'
local bindedattr = require 'binded_attr'
local Helpers = require 'Helpers'
local Placeholders = require 'Placeholders'

local __composer = { }

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
	on_create = function (self)

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

		print(bindedattr)
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
		local multiply_corefmt = Utility.Homogeneous4D(unpack(self:get_datafield 'ColorMultiplyAttr'))

		local element_types = Helpers.scriptType_TechnoRTTITable(self:container_parent())['appearance']['render_elements']

		for i, element in ipairs(element_types) do
			if element.affected_by_global_multiply then
				self:get_container().a['RenderElementsManger']:get_element_named(element.name).colorMultiply = multiply_corefmt
			end
		end
	end,
})

__composer.comp_TechnoColorMultiply = comp_TechnoColorMultiply

-- * COMPONENT: RenderBasicBody *
--
-- dependency:
--
--		* CubeCore -> Silcon
--		* Scripts -> Helpers
--		* Scripts -> composer -> RenderElementsManger
--
-- component datafields:
--
--		none
--
-- component methods:
--
--		none, it is intended to parse the render_elements field in ScriptType.
--
--	used field in TechnoScriptType:
--
--		appearance -> render_elements
--
--	changelog:
--
--		* 2014.10.6 initial commit.

local comp_RenderBasicBody = components.component:new({

	name = "RenderBasicBody",
	alias = "RenderBasicBody",

})

function comp_RenderBasicBody:on_init()
	local scriptType = Helpers.scriptType_TechnoRTTITable(self:container_parent())

	local attr_render_elements = scriptType:property 'appearance' ['render_elements']

	local elements_manger = self:get_container().a['RenderElementsManger']

	for i, element in ipairs(attr_render_elements) do
		local creator = elements_manger:element_internal_creator(element.type_general, element.type_static, element.type_directioned, element)

		local element_body = nil
		local coord_offset = Utility.CoordStruct(unpack(element.offset))

		if element.has_shadow then
			local element_shadow = creator(Helpers.texture(element.shadow), element.shadow_faces)
			element_shadow.UseShadowProjection = true
			element_shadow.colorMultiply = Utility.Homogeneous4D(0.1, 0.1, 0.1, 0.6)
			element_shadow.offset = coord_offset
			if element.type_directioned then
				element_shadow.direction_offset = element.direction_offset
			end
			elements_manger:add_element(10, element.name .. '_shadow', element_shadow)
		end
		
		if element.type_general then
			element_body = creator(Helpers.texture(element.image), element.image_faces)
			element_body.offset = coord_offset
			if element.type_directioned then
				element_body.direction_offset = element.direction_offset
			end
		elseif element.type_internal_line then
			local coord1 = Utility.CoordStruct(unpack(element.point1))
			local coord2 = Utility.CoordStruct(unpack(element.point2))
			local color = Utility.Homogeneous4D(unpack(element.color))

			element_body = creator(coord1, coord2, color)
			element_body.thickness = element.thickness
		end

		elements_manger:add_element(0, element.name, element_body)
	end
end

__composer.comp_RenderBasicBody = comp_RenderBasicBody

-- * COMPONENT: RenderElementsManger *
--
-- dependency:
--
--		* CubeCore -> Silcon
--		* Scripts -> Helpers
--
-- component datafields:
--
--		* elements
--		* elements_dict
--		* get_elements_dict
--		* get_element_named
--
-- component methods:
--
--		* CubeCore::RenderElement add_element(index, name, element)
--		* function(RenderElementCreator) element_interal_creator(istatic, isdirectioned)
--
--	used field in TechnoScriptType:
--
--		none, it just provide a interface to CubeCore->RenderElement
--
--	changelog:
--
--		* 2014.10.4 EVE initial commit.
--		* 2014.10.6 EVE update, add two comp methods
--
--	no on_update currently, only on_create and some comp methods

local comp_RenderElementsManger = components.component:new({

	name = "RenderElementsManger",
	alias = "RenderElementsManger",

	on_create = Placeholders.ComponentMethod,

	add_element = Placeholders.ComponentMethod,

	element_internal_creator = Placeholders.ComponentMethod,

	get_elements_dict = Placeholders.ComponentMethod,

	get_element_named = Placeholders.ComponentMethod,

})

function comp_RenderElementsManger:on_create()
	self:set_datafield('elements', { })
	self:set_datafield('elements_dict', { })
end

function comp_RenderElementsManger:add_element(index, name, element)
	local element_container = Helpers.Techno_TechnoRTTIIDTable(self:container_parent()).elements
	element_container:insert(index, element)
	element.parent = element_container
	Helpers.tblinsert(self:get_datafield 'elements' , element)
	self:get_datafield 'elements_dict' [name] = element
	return element
end

function comp_RenderElementsManger:get_elements_dict() return self:get_datafield 'elements_dict' end

function comp_RenderElementsManger:get_element_named(name) return self:get_datafield 'elements_dict' [name] end

function comp_RenderElementsManger:element_internal_creator(igeneral, istatic, isdirectioned, org_dict)
	if igeneral then
		if istatic and isdirectioned then
			return Utility.RenderElement_DirectionedStatic.create
		elseif (not istatic) and isdirectioned then
			return Utility.RenderElement_FramedDynamic.create
		elseif istatic and (not isdirectioned) then
			return Utility.RenderElement_FramedStatic.create
		elseif (not istatic) and (not isdirectioned) then
			return Utility.RenderElement_FramedDynamic
		end
	else
		if org_dict.type_internal_line then
			return Utility.RenderElement_InternalLine.create
		end
	end
end

__composer.comp_RenderElementsManger = comp_RenderElementsManger

return __composer
