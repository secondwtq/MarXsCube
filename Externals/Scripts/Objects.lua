Import("Anims.lua")

local composer = require('composer')
local gmap_comps = require('gmap_comps')
local locomotor = require 'locomotor'
local blocomotor = require 'loco_bvehicle'
local locomotor_raycast = require 'locomotor_raycast'

local function base_def_technos()

OBJECTS.TESTTECHNO = BASES.BaseTechno:newObject({
	type = "TECHNO",
	clickselectable = true,
	usecollsphere = true,
	rcollsphere = 36,

	appearance = {
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

				image = "TRIKEIMAGE",
				image_faces = 36,
				shadow = "TRIKEIMAGE_SHADOW",
				shadow_faces = 36,

				multiply = { 1.0, 1.0, 1.0, 1.0 },
				affected_by_global_multiply = true,
			}
		},
	},

	components = {
		composer.comp_TechnoColorMultiply,
		composer.comp_RenderElementsManger,
		composer.comp_RenderBasicBody,
		blocomotor.comp_BVehicle,
		locomotor_raycast.comp_locoraycast
	},

	physics = {
		enabled = true,
		initialstatic = false,
		mass = 800,
		offset = { x = 0, y = 0, z = 0 },
		angle_factor = { x = 1,0, y = 1.0, z = 1.0 },
		ntype_locomotor = 'bvehicle',
		vehicle_type = 'raycast',
		raycast_vehicle_args = {
			wheel_friction = 2.0,
			wheel_radius = 20,
			wheel_height = 10,
			wheels = {
				{
					position = { 38, 25, 0 },
					engine = false, brake = false, steer = true
				},
				{
					position = { 38, -25, 0 },
					engine = false, brake = false, steer = true
				},
				{
					position = { -38, 25, 0 },
					engine = true, brake = true, steer = false
				},
				{
					position = { -38, -25, 0 },
					engine = true, brake = true, steer = false
				}
			},
		},
		raycast_locomotor_args = {
			engineforce = 5000,
			stablespeed = 50,
			brake_threshold = 20,
			brake_extrathreshold = 60,
			brakingforce = 50000,
			max_rot_radius = 128,
			rotate_negativeforce = 800,
			extra_rotforce = 10000,
			extra_brakeforce = 16000,
		},
		shape = {
			type = "COMPBOX",
			restitution = 0.2,
			friction = 0.8,
			offset_comp = { x = 0, y = 0, z = 25 },
			offset = { x = 0, y = 0, z = 0 },
			size = { x = 52, y = 30, z = 18 }
		}
	}
})

OBJECTS.TESTTECHNO_PHY = BASES.BaseTechno:newObject({
	type = "TECHNO",
	occupationtype = "centeredynamic",
	occupationfoundation_simple = { x = 1, y = 1 },

	appearance = {
		render_elements = { {
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
		} }
	},

	components = {
		composer.comp_TechnoColorMultiply,
		composer.comp_RenderElementsManger,
		composer.comp_RenderBasicBody,
		locomotor.comp_LocomotorDefault,
	},

	physics = {
		enabled = true,
		initialstatic = false,
		mass = 1600,
		offset = { x = 0, y = 0, z = 48 },
		angle_factor = { x = 1.0, y = 1.0, z = 1.0 },
		ntype_locomotor = 'default',
		nlocomotor_args = {
			rot = 200,
			stablespeed = 200,
			engineforce = 400,
			brakingforce = 500,
			rotate_negativeforce = 300,
		},
		shape = {
			type = "BOX",
			mass = 10.0,
			restitution = 0.2,
			friction = 1,
			size = { x = 72, y = 48, z = 48 }
		}
	}
})

OBJECTS.TESTBUILDING = BASES.BaseTechno:newObject({
	type = "TECHNO",
	render_preset = "structure",
	image = "TESTBUILDING_BODY",
	shadow = "TESTBUILDING_SHADOW",
	clickmoveable = false,
	clickselectable = true,
	occupationtype = "buildingstatic",
	occupationfoundation_simple = { x = 4, y = 4 },

	appearance = {
		render_elements = {
			{
				name = 'bodybuilding',

				type_general = true,
				type_static = true,
				type_directioned = false,
				type_internal_line = false,
				has_shadow = true,

				offset = { 0, 0, 0 },
				direction_offset = 0,

				image = "TESTBUILDING_BODY",
				image_faces = 32,
				shadow = "TESTBUILDING_SHADOW",
				shadow_faces = 32,

				multiply = { 1.0, 1.0, 1.0, 1.0 },
				affected_by_global_multiply = true,
			}
		},
	},

	physics = {
		enabled = true,
		initialstatic = true,
		mass = 5.0,
		offset = { x = 128, y = 128, z = 64 },
		angle_factor = { x = 1.0, y = 1.0, z = 1.0 },
		locomotor_type = "default",
		locomotor_args = {
			realtimeupdate = true,
			balance_force_mult = 10.0,
			rot = 1,
			stablespeed = 200,
			engineforce = 400,
			rot_negativeforce = 50,
		},
		shape = {
			type = "BOX",
			mass = 10.0,
			restitution = 0.0,
			friction = 1,
			size = { x = 128, y = 128, z = 54 }
		}
	}
})

OBJECTS.GRAPH_LINE = BASES.BaseTechno:newObject({
	type = "TECHNO",
	clickmoveable = false,
	clickselectable = false,
	occupationtype = 'none',

	appearance = {
		render_elements = {
			{
				name = 'line',
				type_general = false,
				type_internal_line = true,
				has_shadow = false,
				point1 = { 0, 1000, 50 },
				point2 = { 1000, 0, 50 },
				offset = { 0, 0, 0 },
				thickness = 2,
				color = { 1, 1, 1, 0.5 }
			},
		}
	},

	components = {
		-- composer.comp_TechnoColorMultiply,
		composer.comp_RenderElementsManger,
		composer.comp_RenderBasicBody,
		gmap_comps.comp_GraphLineStore,
	},


	physics = {
		enabled = false,
		never_enable = true,
	}

})

OBJECTS.GRAPH_NODE = BASES.BaseTechno:newObject({
	type = "TECHNO",
	clickmoveable = false,
	clickselectable = false,
	occupationtype = 'none',

	appearance = {
		render_elements = {
			{
				name = 'body',
				type_general = true,
				type_directioned = false,
				type_static = true,
				has_shadow = true,
				image = "GRAPH_NODE_IMAGE",
				shadow = "GRAPH_NODE_SHADOW",
				offset = { 0, 0, 0 },
				multiply = { 1.0, 1.0, 1.0, 1.0 },
				affected_by_global_multiply = true,
			}
		}
	},

	components = {
		composer.comp_RenderElementsManger,
		composer.comp_RenderBasicBody,
		gmap_comps.comp_GraphNodeStore,
	},

	physics = {
		enabled = false,
		never_enable = true,
	}

})

OBJECTS.SATELITE_BG = BASES.BaseTechno:newObject({
	type="TECHNO",
	clickmoveable=false,
	clickselectable=false,
	occupationtype='none',

	appearance={
		render_elements={
			{
				name = 'body',
				type_general = true,
				type_directioned = false,
				type_static = true,
				has_shadow = false,
				image = "SATELITE",
				shadow = "SATELITE",
				offset = { 0, 0, 0 },
				multiply = { 1.0, 1.0, 1.0, 1.0 },
				affected_by_global_multiply = true,
			}
		},
	},

	components = {
		composer.comp_RenderElementsManger,
		composer.comp_RenderBasicBody,
	},

	physics={enabled=false}
})

end

base_def_technos()
