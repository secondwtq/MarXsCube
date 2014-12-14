#ifndef CUBE_E_CONFIG_H
#define CUBE_E_CONFIG_H

#include "Common.h"
#include <LuaJit_Bridge.h>
#include <vector>
#include <string>
#include <iostream>

class Abs_Abstract;
class LuaStatus;

typedef void (* LuaLoadCallback) (LuaStatus&);

class ConfigManger {
	public:
		LuaStatus *ConfigState = nullptr;
		ConfigManger(LuaStatus &state);
		void loadConfigFrom(const char *filename);
		void load() { }
		static void LUA_loadTexture(ConfigManger *self, luabridge::LuaRef name, luabridge::LuaRef args) {LOGFUNC; self->loadTexture(name, args); }
		static void LUA_loadObject(ConfigManger *self, luabridge::LuaRef name, luabridge::LuaRef args) {LOGFUNC; self->loadObject(name, args); }

		void loadTexture(std::string name, luabridge::LuaRef args);
		void loadObject(std::string name, luabridge::LuaRef args);

		luabridge::LuaRef getObject(std::string name);
};

class LuaStatus {
	public:
		lua_State *State = nullptr;
		bool _loaded = false;
		std::vector<std::string> files;
		operator lua_State *() {LOGFUNC; return State; }

		void load(const char *filename, LuaLoadCallback callback = nullptr);

		bool loaded(const char *filename);

		void init();

		void dispose() {LOGFUNC;
			if (_loaded) lua_close(State);
		}

};

class EventManger {
	public:
		static EventManger *instance;
		static inline EventManger &GetInstance() {LOGFUNC; return *instance; }

		const static size_t EventCounts = 15;
		enum Events {
			ANIM_UPDATE,
			ANIM_CREATE,
			TEST_BEGIN,
			GAME_UPDATE_BEGIN,
			ANIM_SPAWN,
			UI_KEYPRESS,
			UI_KEYRELEASE,
			UI_MOUSEPRESS,
			UI_MOUSERELEASE,
			UI_MOUSEMOVE,
			TECHNOTYPE_LOAD,
			TECHNO_UPDATE,
			TECHNO_SPAWN,
			TECHNO_CREATE,
			TECHNO_PHYUPDATE
		};

		LuaStatus *_state = nullptr;
		luabridge::LuaRef FunctionTable;
		luabridge::LuaRef ObjectsTable;
		// luabridge::LuaRef LuaObjects[EventCounts];

		std::vector<luabridge::LuaRef> _Events;
		std::vector<std::string> _EventsName;

		luabridge::LuaRef CreateObjectTable(Abs_Abstract &src);
		luabridge::LuaRef GetObjectTable(Abs_Abstract &src);
		void DestroyObjectTable(Abs_Abstract &src);

		luabridge::LuaRef &GetEvent(Events type);

		EventManger(LuaStatus &State);
	private:
		void inline addEvent(std::string funcName) {LOGFUNC;  _Events.push_back(FunctionTable[funcName]); _EventsName.push_back(funcName); }
};

#include "Abstract.h"

#endif