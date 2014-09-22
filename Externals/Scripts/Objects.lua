Import("Anims.lua")

OBJECTS.TESTTECHNO = BASES.BaseTechno:newObject({
	type = "TECHNO",
	imagefaces = 36,
	shadowimagefaces = 36,
	image = "TRIKEIMAGE",
	shadow = "TRIKEIMAGE_SHADOW",
	clickselectable = true,
	usecollsphere = true,
	rcollsphere = 36,

	physics = {
		enabled = true,
		initialstatic = false,
		mass = 0.5,
		offset = { x = 0, y = 0, z = 24 },
		angle_factor = { x = 1.0, y = 1.0, z = 1.0 },
		locomotor_type = "default",
		locomotor_args = {
			realtimeupdate = true,
			balance_force_mult = 1,
			rot = 6,
			stablespeed = 650,
			engineforce = 60,
			rot_negativeforce = 2,
		},
		shape = {
			type = "BOX",
			mass = 0.5,
			restitution = 0.5,
			friction = 0.3,
			offset = { x = 0, y = 0, z = 0 },
			size = { x = 48, y = 18, z = 24 }
		}
	}
})

OBJECTS.TESTTECHNO_PHY = BASES.BaseTechno:newObject({
	type = "TECHNO",
	image = "TESTUNITBODYIMAGE",
	shadow = "TESTUNITSHADOWIMAGE",
	occupationtype = "centeredynamic",
	occupationfoundation_simple = { x = 1, y = 1 },

	physics = {
		enabled = true,
		initialstatic = false,
		mass = 5.0,
		offset = { x = 0, y = 0, z = 48 },
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