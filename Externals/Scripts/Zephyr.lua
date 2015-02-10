stdio = terralib.includec("stdio.h")
zcommon = terralib.includec("MarXsCube/ZephyrCommon.h")
zcore = terralib.includec("MarXsCube/ZephyrCore.h")

terra zephyr_check_output_user()
	stdio.printf("CoordStruct: %lu\n", terralib.sizeof(zcommon.ZCoordStruct));
	stdio.printf("LuaRef: %lu\n", terralib.sizeof(zcommon.ZephyrScriptRef));
	stdio.printf("Abs_Abstract: %lu\n", terralib.sizeof(zcore.ZephyrObject));
	stdio.printf("Abs_Anim: %lu\n", terralib.sizeof(zcore.ZephyrAnim));
	stdio.printf("Abs_Object: %lu\n", terralib.sizeof(zcore.ZephyrObject_));
	stdio.printf("Abs_Techno: %lu\n", terralib.sizeof(zcore.ZephyrTechno));
end

zephyr_check_output_user()