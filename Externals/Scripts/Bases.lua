Import("_Base.lua")

local composer = require 'composer'

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

	physics = {
		enabled = false,
		mass = 0.0
	},

	components = {
		composer.comp_TechnoColorMultiply
	},

	_image = nil,
	_shadowimage = nil,
})

BASES.BaseObject = nil
