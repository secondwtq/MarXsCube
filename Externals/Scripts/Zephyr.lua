stdio = terralib.includec("stdio.h")
zcommon = terralib.includec("MarXsCube/ZephyrCommon.h")
zcore = terralib.includec("MarXsCube/ZephyrCore.h")

terra zephyr_check_output_user()
	stdio.printf("CoordStruct: %lu\n", terralib.sizeof(zcommon.ZCoordStruct));
	stdio.printf("LuaRef: %lu\n", terralib.sizeof(zcommon.ZephyrScriptRef));
	stdio.printf("Userdata: %lu\n", terralib.sizeof(zcommon.ZephyrObjectRef_));
	stdio.printf("Abs_Abstract: %lu\n", terralib.sizeof(zcore.ZephyrObject));
	stdio.printf("Abs_Anim: %lu\n", terralib.sizeof(zcore.ZephyrAnim));
	stdio.printf("Abs_Object: %lu\n", terralib.sizeof(zcore.ZephyrObject_));
	stdio.printf("Abs_Techno: %lu\n", terralib.sizeof(zcore.ZephyrTechno));
end

terra get_location(techno : &zcore.ZephyrTechno)
	stdio.printf("%x\n", techno)
	stdio.printf("Location: %d %d %d\n", techno.parent.location.x, techno.parent.location.y, techno.parent.location.z)
end

terra get_location_ex(technor : zcore.ZephyrObjectRef)
	var techno = [&zcore.ZephyrTechno](technor.ref)
	stdio.printf("Location: %d %d %d\n", techno.parent.location.x, techno.parent.location.y, techno.parent.location.z)
end

zephyr_check_output_user()