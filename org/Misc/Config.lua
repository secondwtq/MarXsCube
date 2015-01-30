Environment = { }
Environment.ConfigPath = ".\\Scripts\\"
Import =
	function(s)
		Utility.DoImport(Environment.ConfigPath .. s)
	end

Import("Debug.lua")

Import("CommonDef.lua")

TEXTURES = config.textures
OBJECTS = config.objects
BASES = config.bases

Import("Bases.lua")

Import("Textures.lua")

Import("Objects.lua")

Import("GeneralEnv.lua")

Import("Functions.lua")

Import("Test.lua")

Import("_loadFunctions.lua")