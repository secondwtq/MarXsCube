config = { }

config.bases = { }
config.textures = { }
config.objects = { }
config.util = { }

Functions = { }
Environment.ObjectTable = { }

Enums = { }

Enums.General = { }

Enums.Direction = {
	North = 0x0,
	NorthEast = 0x1,
	East = 0x2,
	SouthEast = 0x3,
	South = 0x4,
	SouthWest = 0x5,
	West = 0x6,
	NorthWest = 0x7,
}

Enums.RTTITypeID = {
	Abstract = 0,
	Cell = 1,
	Object = 2,
	AbstractType = 3,
	ObjectType = 4,
	Anim = 5,
	AnimType = 0x6,
	Techno = 0x7,
	TechnoType = 0x8,
	TechnoRTTIIDTable = 0x9,
}

Enums.General.CameraKeyMoveStep = 20.0
Enums.General.CameraMouseSlideStep = 16.0

Import("KeyDef.lua")
Import("EventsDef.lua")
