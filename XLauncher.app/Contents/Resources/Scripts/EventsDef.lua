Functions._defaultFuns = { }

Functions._defaultFuns.noArg = function() end
Functions._defaultFuns.singleArg = function(x) end
Functions._defaultFuns.doubleArg = function(x, y) end



Functions.Session_KeyPress = Functions._defaultFuns.singleArg
Functions.Session_KeyRelease = Functions._defaultFuns.singleArg
Functions.Session_MousePress = Functions._defaultFuns.singleArg
Functions.Session_MouseRelease = Functions._defaultFuns.singleArg
Functions.Session_MouseMove = Functions._defaultFuns.singleArg

Functions.Abs_Anim_onCreate = Functions._defaultFuns.doubleArg
Functions.Abs_Anim_onSpawn = Functions._defaultFuns.doubleArg
Functions.Abs_Anim_onUpdate = Functions._defaultFuns.doubleArg

Functions.Main_GameUpdateBegin = Functions._defaultFuns.noArg
Functions.TestManger_onTestInit = Functions._defaultFuns.noArg

Functions.Abs_Techno_onUpdate = Functions._defaultFuns.doubleArg
Functions.TechnoType_onLoad = Functions._defaultFuns.doubleArg
Functions.Abs_Techno_onSpawn = Functions._defaultFuns.doubleArg
Functions.Abs_Techno_onCreate = Functions._defaultFuns.doubleArg

Functions.Abs_Techno_onPhysicsTransformed = Functions._defaultFuns.doubleArg

Functions._defaultFuns = nil