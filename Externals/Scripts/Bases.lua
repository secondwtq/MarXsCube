
local composer = require 'composer'
local Placeholders = require 'Placeholders'

Import("_Base.lua")

BASES.BaseTexture = BASES.BaseBase:newObject({
	filename = "./Textures/dummy.png",
	frame_count = 1,
	frame_row = 1,
	frame_col = 1,
	useExtraTexture = false,
})

BASES.BaseAnim = BASES.BaseObject:newObject({
	type = "ANIM",
	image = "TESTANIM_TEX",
	loop_count = -1,
	frame_step = 1,
	frame_last = 1,
	offset = { x = 0, y = 0 },
	scale = { x = 100, y = 100 },

	next = nil,
})

BASES.BaseTechno = BASES.BaseObject:newObject({
	type = "TECHNO",
	imagefaces = 32,
	shadowimagefaces = 32,
	render_preset = "vehicle",
	image = "TESTUNITBODYIMAGE",
	shadow = "TESTUNITSHADOWIMAGE",
	clickmoveable = true,
	clickselectable = true,
	occupationtype = "local",
	usecollsphere = true,
	rcollsphere = 64,

	appearance = {

		render_preset = 'vehicle',

		render_elements = {

			{
				name = 'body',

				type_general = true,
				type_static = true,
				type_directioned = true,
				type_internal_line = false,
				has_shadow = true,

				offset = { 0, 0, 0 },
				direction_offset = 0,

				image = "TESTUNITBODYIMAGE",
				image_faces = 32,
				shadow = "TESTUNITSHADOWIMAGE",
				shadow_faces = 32,

				multiply = { 1.0, 1.0, 1.0, 1.0 },
				affected_by_global_multiply = true,
			}

		},

	},

	physics = {
		enabled = false,
		mass = 0.0
	},

	components = {
		composer.comp_TechnoColorMultiply,
		composer.comp_RenderElementsManger,
		composer.comp_RenderBasicBody,
	},

	_image = nil,
	_shadowimage = nil,

	-- well you are suggested to use syntax like
	--	TechnoScriptType:property '<property>' to index primary properties
	property = Placeholders.ComponentMethod,

})

function BASES.BaseTechno:property(key)
	return self[key]
end

BASES.BaseObject = nil
