#include "Config.h"
#include "ObjectManger.h"
#include <algorithm>
#include <cctype>
using namespace luabridge;
using namespace std;

#include "Type_Anim.h"
#include "Type_Techno.h"
#include "Anim.h"

EventManger *EventManger::instance = nullptr;

void ConfigLoadCallback(LuaStatus &L) {
	LOGFUNC;
	getGlobalNamespace(L).
		beginNamespace("Host").
			beginClass<ConfigManger>("ConfigManger").
				addStaticFunction("loadTexture", &ConfigManger::LUA_loadTexture).
				addStaticFunction("loadObject", &ConfigManger::LUA_loadObject).
			endClass().
		endNamespace();
}

namespace {
	void LUA_ImportFile(const char *filename, lua_State *L) {
		LOGFUNC;
		cout << "CubeCore: Config:: ::LUA_ImportFile - (Initial Import) Including script " << filename << " ..." << endl;
		if (luaL_dofile(L, filename))
			cout << "CubeCore: Config:: ::LUA_ImportFile - luaL_dofile: Error in script " << filename << "!" << endl;
	}
}

void LuaStatus::init() {LOGFUNC;
	if (!_loaded) {
		cout << "CubeCore: LuaStatus::init - Initing luaState... " << endl;
		State = luaL_newstate();
		luaL_openlibs(State);
		getGlobalNamespace(State).beginNamespace("Utility").addFunction("DoImport", &LUA_ImportFile).endNamespace();
		_loaded = true;
	}
}

bool LuaStatus::loaded(const char *filename) {LOGFUNC;
	std::string s_name(filename);
	std::transform(s_name.begin(), s_name.end(), s_name.begin(), ::tolower);
	return (find(files.begin(), files.end(), s_name) != files.end());
}

void LuaStatus::load(const char *filename, LuaLoadCallback callback) {LOGFUNC;
	printf("Loading %s\n", filename);
	if (_loaded) {
		std::string s_name(filename);
		std::transform(s_name.begin(), s_name.end(), s_name.begin(), ::tolower);
		if (find(files.begin(), files.end(), s_name) == files.end()) {
			if (luaL_dofile(State, filename))
				cout << "CubeCore: LuaStatus::load - luaL_dofile: Error in script " << filename << "!" << endl;
			files.push_back(s_name);
		}
		else std::cout << "Config file " << filename << " is duplicated." << std::endl;
	}
	if (callback) callback(*this);
	cout << "CubeCore: LuaStatus::load - Initing EventManger::instance... " << endl;
	EventManger::instance = new EventManger(*this);
}

ConfigManger::ConfigManger(LuaStatus &state) : ConfigState(&state) {LOGFUNC;
	printf("ConfigManger constructed.\n");
}

void ConfigManger::loadObject(string name, LuaRef args) {LOGFUNC;
	cout << "CubeCore: ConfigManger::loadObject - Loading " << name << " ..." << endl;
	string tp = args["type"];
	if (tp == "ANIM") {
		Type_Anim::Array.FindOrLoad(name, *this);
	} else if (tp == "TECHNO") {
		Type_Techno::Array.FindOrLoad(name, *this);
	}
}

LuaRef ConfigManger::getObject(std::string name) {LOGFUNC;
	LuaRef loadFoo = getGlobal(*ConfigState, "getObjectDef");
	return loadFoo(name);
}

void ConfigManger::loadTexture(string name, luabridge::LuaRef args) {LOGFUNC;
	cout << "CubeCore: ConfigManger::loadTexture - Loading " << name << " [" << (int)(args["frame_count"]) << "] ..." << endl;
	auto ret = new TextureAtlas(args["filename"], (int)(args["frame_count"]), (int)(args["frame_row"]), (int)(args["frame_col"]));
	ret->useExtra = args["useExtraTexture"];
	ret->frames.push_back((int)(args["frame_count"]));
	ret->totalTexNum = (int)(args["frame_count"]);
	if (ret->useExtra) {
		ret->extraCount = args["extra_num"];
		LuaRef r = args["extra"];
		for (int i = 1; i <= ret->extraCount; i++){
			LuaRef _r = r[i];
			sf::Texture rt;
			string filename = _r[2];
			rt.loadFromFile(filename);
			ret->extras.push_back(rt);
		}
		for (int i = 1; i <= ret->extraCount; i++){
			LuaRef _r = r[i];
			sf::Texture rt;
			int f = _r[1];
			ret->totalTexNum += f;
			ret->frames.push_back(f+ret->frames[i-1]);
		}
	}
	TextureManger::GetInstance().TextureHashs[name] = ret;
}

void ConfigManger::loadConfigFrom(const char *filename) {LOGFUNC;
	printf("Loading config from %s\n", filename);
	ConfigState->load(filename, ConfigLoadCallback);
	LuaRef loadFoo = getGlobal(*ConfigState, "load");
	printf("CubeCore: ConfigManger::loadConfigFrom - Calling load function..\n");
	loadFoo(this);
	printf("CubeCore: ConfigManger::loadConfigFrom - Load Function finished..\n");
}

luabridge::LuaRef &EventManger::GetEvent(Events type) {LOGFUNC; Debug::logstream << _EventsName[type] << endl; return _Events[type]; }

#define ADD_EVENT(init, func) case init: addEvent(func); break

EventManger::EventManger(LuaStatus &State) : _state(&State),FunctionTable(getGlobal(State, "Functions")), ObjectsTable(getGlobal(State, "Environment")["ObjectTable"]) {
	LOGFUNC;
	cout << "CubeCore: EventManger::EventManger - Constructing... " << endl;
	for (size_t i = 0; i < EventCounts; i++) switch (i) {
				ADD_EVENT(ANIM_UPDATE, "Abs_Anim_onUpdate");
				ADD_EVENT(ANIM_CREATE, "Abs_Anim_onCreate");
				ADD_EVENT(TEST_BEGIN, "TestManger_onTestInit");
				ADD_EVENT(GAME_UPDATE_BEGIN, "Main_GameUpdateBegin");
				ADD_EVENT(ANIM_SPAWN, "Abs_Anim_onSpawn");
				ADD_EVENT(UI_KEYPRESS, "Session_KeyPress");
				ADD_EVENT(UI_KEYRELEASE, "Session_KeyRelease");
				ADD_EVENT(UI_MOUSEPRESS, "Session_MousePress");
				ADD_EVENT(UI_MOUSERELEASE, "Session_MouseRelease");
				ADD_EVENT(UI_MOUSEMOVE, "Session_MouseMove");
				ADD_EVENT(TECHNOTYPE_LOAD, "TechnoType_onLoad");
				ADD_EVENT(TECHNO_UPDATE, "Abs_Techno_onUpdate");
				ADD_EVENT(TECHNO_SPAWN, "Abs_Techno_onSpawn");
				ADD_EVENT(TECHNO_CREATE, "Abs_Techno_onCreate");
				ADD_EVENT(TECHNO_PHYUPDATE, "Abs_Techno_onPhysicsTransformed");
	}
}

#undef ADD_EVENT

LuaRef EventManger::CreateObjectTable(Abs_Abstract &src) {LOGFUNC;
	// cout << "CubeCore: EventManger::CreateObjectTable - Creating " << src.RTTIID << " ... " << endl;
	ObjectsTable[src.RTTIID] = Generic::CreateObjectTable(src.RTTIID); // newTable(*_state);
	return ObjectsTable[src.RTTIID];
}

LuaRef EventManger::GetObjectTable(Abs_Abstract &src) {LOGFUNC;
	return ObjectsTable[src.RTTIID];
}

void EventManger::DestroyObjectTable(Abs_Abstract &src) {LOGFUNC;
	// ObjectsTable[src.RTTIID] = Nil();
}