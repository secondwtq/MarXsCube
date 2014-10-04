Environment = { }
Environment.ConfigPath = "Scripts/"

package.path = package.path .. ';./ScriptsX/?.lua;./Scripts/?.lua'

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
