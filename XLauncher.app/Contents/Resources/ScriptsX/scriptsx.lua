dofile('ScriptsX/binded_attr.lua')

-- print(package.path)
-- table.insert(package.path, '/Users/secondatke/Documents/branches/MarXsCube/Externals/ScriptsX')
package.path = package.path .. ';./ScriptsX/?.lua'
-- print(package.path)

-- bindedattr = require 'binded_attr'
bindedattr = _binded_attr