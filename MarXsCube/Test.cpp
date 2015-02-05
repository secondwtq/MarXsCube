#include "Common.h"
#include "SFML.h"
#include <vector>
#include "ObjectManger.h"
#include "Map.h"
#include "Session.h"
#include "Object.h"
#include "Config.h"
#include "LuaInterface.h"
#include "LuaUtils.h"
#include "Pathfinder.h"
#include "Generic.h"

#include "CubeTransform.h"

#include "FSM.h"
#include "ATVBCube.h"
using namespace FSMHelper;
using namespace ATVBCube::Helper;
using namespace sf;
using namespace std;

#include "Atheverybeginning.h"
#include "Startup.h"

#include "BulletThread.h"
#include "SilconThread.h"
#include "GLFoundation.h"
#include "WonderlandFoundation.h"

sf::RenderWindow *window_global = nullptr;

void safe_session_close() {
	Acheron::Bullet.invoke_and_stop();
	Acheron::Silcon.invoke_and_stop();
	TestManger::GetInstance().window->close();
}

int main() {

	cube_init_first();
	Generic::Init_LuaStatus();
	Generic::Init_Session();
	cube_init_atvb();
	ATVBCube::load<S::TilerGeneralSetting>();

	cube_init_window();
	
	ConfigManger config(*Generic::lua_state());
	LuaInterface::RegisterInterface(*Generic::lua_state());
	LuaUtils::initLuaUtils();
	Generic::Init_FunObjectTableCreate_Forward(config);
	Generic::Init_RenderLayerManger();
	Generic::Init_PhysicsGeneral();
	Generic::init_TilerRenderingManger();
	config.loadConfigFrom("Config.lua");
	
	init_opengl();
	
	Generic::Init_FunObjectTableCreate(config);

	TestManger::GetInstance().window = window_global;
	TestManger::GetInstance().initTest();

	Pathfinding::init(Map::GetInstance().size.x, Map::GetInstance().size.y);
	obsTransform::UpdateVm(0, 0);
	CubeTransform::generate_view_matrix(0, 0);
	
	Acheron::Bullet.start(Acheron::SYNCSTATE::SYNCED);
	Acheron::Silcon.start(Acheron::SYNCSTATE::SYNCED_NONAUTO);
	Acheron::Silcon.invoke();
	
	sf::Event event;
	
	while (window_global->isOpen()) {

		Acheron::Bullet.pause();
		
		EventManger::GetInstance().GetEvent(EventManger::Events::GAME_UPDATE_BEGIN)();

		Generic::Session()->setMousePos_Ab();
		Generic::Session()->updateMouseButtonStatus();
		
		while (window_global->pollEvent(event)) {
			switch (event.type) {
				case Event::Closed: safe_session_close(); break;

				case Event::KeyPressed:
					Generic::Session()->setKeyEvent(event.key);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_KEYPRESS)(Generic::Session()->getKeyEvent());
					break;

				case Event::KeyReleased:
					Generic::Session()->setKeyEvent(event.key);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_KEYRELEASE)(Generic::Session()->getKeyEvent());
					break;

				case Event::MouseButtonPressed:
					Generic::Session()->setMousePos_Press(event.mouseButton);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_MOUSEPRESS)(Generic::Session()->getMousePos());
//					raise_verts();
					tiler_array_test();
					break;

				case Event::MouseButtonReleased:
					Generic::Session()->setMousePos_Release(event.mouseButton);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_MOUSERELEASE)(Generic::Session()->getMousePos());
					break;

				case Event::MouseMoved:
					Generic::Session()->setMousePos_Rl(event.mouseMove);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_MOUSEMOVE)(Generic::Session()->getMousePos());
					break;

				default: break;
			}
		}

		for (auto i : ObjectManger::GetInstance().Objects)
			i->Update();
		
		ObjectManger::GetInstance().FinishRemove();

		Acheron::Bullet.invoke();
	}
	
	Generic::Dispose_PhysicsGeneral();
	Generic::Dispose_Logger_n_FSM();

	return 0;
}