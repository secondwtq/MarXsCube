-- this file is part of Project MarXsCube Prototype.

-- subproject:
--
--		Lunar?
--		ScriptX?
--
-- placeholders for declearation inside table?
--
-- including:
--
--		* Placeholders.ComponentMethod
--
-- dependency:
--
--		none, as it implies, it is just some placeholders.
--
--	use in other modules:
--
--		Placeholders = require 'Placeholders'
--
--	changelog:
--
--		* 2014.10.5 initial commit
--
--	http://github.com/secondwtq/MarXsCube

local __Placeholders = { }

local function placeholder_foo () end

__Placeholders.ComponentMethod = placeholder_foo

__Placeholders.EmptyFunction = placeholder_foo

return __Placeholders
