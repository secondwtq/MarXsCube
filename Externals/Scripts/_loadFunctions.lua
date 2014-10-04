function loadTextures(manager)
	for k, v in pairs(config.textures) do
		Host.ConfigManger.loadTexture(manager, k, v) 
	end
end

function getObjectDef(id)
	return OBJECTS[id]
end

function loadObjects(manager)
	for k, v in pairs(config.objects) do Host.ConfigManger.loadObject(manager, k, v) end
end

function loadObjectsLater()
	for k, v in pairs(config.objects) do
		if v.type == "ANIM" then
			if v.next ~= nil then
				v.next = getObjectDef(v.next)
			end
		end
	end
end

function load(manager)
	loadTextures(manager)
	loadObjects(manager)

	loadObjectsLater()
end

function CubeCore_CreateObjectTable(RTTIID)
	local ret = { }
	ret.__RTTIID = RTTIID
	return ret
end
