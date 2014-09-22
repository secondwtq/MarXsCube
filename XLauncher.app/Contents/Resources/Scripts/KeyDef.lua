Enums.Key = {
	Unknown = -1,
	Esc = 36,
	Left = 71,
	Right = 72,
	Up = 73,
	Down = 74
}

Enums._Key = { }

Enums._Key._setKey = function(c, n) Enums.Key[c] = n end

Enums.General.LetterUppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
Enums.General.LetterLowers = "abcdefghijklmnopqrstuvexyz"

for i = 0, 9 do Enums._Key._setKey(tostring(i), 26+i) end

Enums._Key._setKey4LET = function(s) for i = 1, #s do Enums._Key._setKey(tostring(s:sub(i, i)), i-1) end end
Enums._Key._setKey4LET(Enums.General.LetterLowers)
Enums._Key._setKey4LET(Enums.General.LetterUppers)

Enums.KeyString = { }

for k, v in pairs(Enums.Key) do Enums.KeyString[v] = k end

Enums._Key = nil