Import("Anims.lua")

local composer = require('composer')
local locomotor = require 'locomotor'
local blocomotor = require 'loco_bvehicle'
local locomotor_raycast = require 'locomotor_raycast'
local pfobstacle = require 'pfobstacle'
local sobstacle = require 'steer_obstacle'
local steer_locomotor = require 'locomotor_steering_raycast'

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

				offset = { 0, 0, -8 },
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
		locomotor_raycast.comp_locoraycast,
		steer_locomotor.comp_LocomotorSteer_Raycast
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
			wheel_friction = 5.0,
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
			engineforce = 4800,
			stablespeed = 60,
			brake_when_turning = true,
			use_aux_rot = true,

			brake_threshold = 20,
			brake_extrathreshold = 60,
			brakingforce = 50000,
			max_rot_radius = 128,
			rotate_negativeforce = 800,
			extra_rotforce = 10000,
			extra_brakeforce = 16000,
			lookahead_offset = 25,

			arrival_radius = 64,
		},
		shape = {
			type = "COMPBOX",
			restitution = 0.2,
			friction = 0.0,
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

				offset = { 0, 0, -12 },
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
		blocomotor.comp_BVehicle,
		steer_locomotor.comp_LocomotorSteer_Raycast
	},

	physics = {
		enabled = true,
		initialstatic = false,
		mass = 1600,
		offset = { x = 0, y = 0, z = 0 },
		angle_factor = { x = 1.0, y = 1.0, z = 1.0 },
		ntype_locomotor = 'bvehicle',
		vehicle_type = 'raycast',
		raycast_vehicle_args = {
			wheel_friction = 2.8,
			wheel_radius = 28,
			wheel_height = 10,
			wheels = {
				{
					position = { 62, 42, 0 },
					engine = false, brake = true, steer = true
				},
				{
					position = { 64, -42, 0 },
					engine = false, brake = true, steer = true
				},
				{
					position = { 0, 42, 0 },
					engine = true, brake = true, steer = true
				},
				{
					position = { 0, -42, 0 },
					engine = true, brake = true, steer = true
				},
				{
					position = { -64, 42, 0 },
					engine = true, brake = true, steer = false
				},
				{
					position = { -64, -42, 0 },
					engine = true, brake = true, steer = false
				}
			},
		},
		raycast_locomotor_args = {
			engineforce = 5400,
			stablespeed = 20,
			brake_when_turning = false,
			use_aux_rot = true,

			brake_threshold = 12,
			brake_extrathreshold = 15,
			brakingforce = 50000,
			max_rot_radius = 48,
			rotate_negativeforce = 0,
			extra_rotforce = 12000,
			extra_brakeforce = 0,
			lookahead_offset = 30
		},
		shape = {
			type = "COMPBOX",
			restitution = 0.2,
			friction = 0.0,
			offset_comp = { x = 0, y = 0, z = 30 },
			offset = { x = 0, y = 0, z = 0 },
			size = { x = 72, y = 56, z = 30 }
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

	components = {
		composer.comp_TechnoColorMultiply,
		composer.comp_RenderElementsManger,
		composer.comp_RenderBasicBody,
		pfobstacle.comp_PathfindingObstacle,
		sobstacle.comp_SteeringObstacle
	},

	pathfinding_obstacle = {
		enabled = true,
		static = true,
		shape = "RECT",
		width = 256,
		height = 256,
		offset = { 128, 128, 0 }
	},

	steering_obstacle = {
		enabled = true,
		static = true,
		radius = 182,
		offset = { 128, 128, 0 }
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
			friction = 0.1,
			size = { x = 128, y = 128, z = 64 }
		}
	}
})

end

base_def_technos()
