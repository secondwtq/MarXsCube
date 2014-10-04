-- this file is part of Project MarXsCube Prototype.

-- subproject:
--
--		Lunar?
--
-- basic support of components
--
-- including:
--
--		* several helper function to use in scripting game logic
--				including wrappers for MarXsCube->CoreHelpers
--
-- dependency:
--
--		* CubeCore->Interfaces->CoreHelpers
--
--	use in other modules:
--
--		Helpers = require 'Helpers'

--
--	changelog:
--
--		* 2014.10.4 PM initial commit.
--
--	http://github.com/secondwtq/MarXsCube

local __Helpers = { }

function __Helpers.texture(name)
	return CoreHelpers.texture(name)
end

return __Helpers