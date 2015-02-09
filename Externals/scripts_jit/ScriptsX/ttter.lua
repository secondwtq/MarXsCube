-- this file is part of Project MarXsCube Prototype.

-- subproject:
--
--		Lunar?
--		ScriptX?
--
-- basic support of getter/setters
--		idea from http://github.com/joshtynjala/getset.lua
--
-- including:
--
--		* ttter.property(table, key, getter, setter) function
--
-- dependency:
--
--		none
--
--	use in other modules:
--
--		ttter = require 'ttter'
--
--	changelog:
--
--		* 2014.10.4 initial commit.
--
--	http://github.com/secondwtq/MarXsCube

--	well, I just found it is one of the very few scripts
--		without the fucking oo instead of lua original style.


--	use lua metatable __index & __newindex for impl.
--	will create a __ttter field in table
--	and might limit other application of metatable
--	but it should be ok to use with lobject
--		wait, it's really okay???

--	the default getter/setter will create a _<Key> field in original table.

local __ttter = { }

local function ready4ttter(table)
	if table.__ttter ~= nil then return table end

	local mt = getmetatable(table)

	if mt ~= nil then

		local org_index = mt.__index
		local org_newindex = mt.__newindex

	else

		mt = { }
		local org_index
		local org_newindex

	end

	rawset(table, '__ttter', {

		org_index = org_index,
		org_newindex = org_newindex,

		props = { }

	})

	local new_index = function (table, key)
		local __ttter = rawget(table, '__ttter')
		if __ttter.props[key] ~= nil then
			return __ttter.props[key].get(table)
		elseif __ttter.org_index ~= nil then
			return __ttter.org_index(table, key)
		end

		return nil
	end

	local new_newindex = function (table, key, value)
		local __ttter = rawget(table, '__ttter')

		if __ttter.props[key] ~= nil then
			return __ttter.props[key].set(table, value)
		elseif __ttter.org_newindex ~= nil then
			return __ttter.org_newindex(table, key, value)
		end

		return nil
	end

	mt.__index, mt.__newindex = new_index, new_newindex
	setmetatable(table, mt)


end

function __ttter.property(table, key, getter, setter)

	ready4ttter(table)

	if getter == nil then
		getter = function (table)
			return table['_' .. key] end
	end

	if setter == nil then
		setter = function (table, value)
			table['_' .. key] = value end
	end

	table.__ttter.props[key] = { get = getter, set = setter }
	rawset(table, key, nil)

	return table

end

return __ttter
