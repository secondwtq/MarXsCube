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
using namespace sf;
using namespace std;

#include "unistd.h"

#include "Atheverybeginning.h"

#include "BulletThread.h"
#include "SilconThread.h"

sf::RenderWindow *window_global = nullptr;

std::size_t global_counter = 0;

extern sf::Clock clock_render;

namespace {
	const std::size_t BUFSIZE_WORKINGDIR_PATH = 1024;
	char workingdir_buffer[BUFSIZE_WORKINGDIR_PATH] { 0 };
}

void safe_session_close() {
	Acheron::Bullet.invoke_and_stop();
	Acheron::Silcon.invoke_and_stop();
	TestManger::GetInstance().window->close();
}

#include "TilerBullet.h"
#include "GLFoundation.h"

void cube_init_workingdir();

void cube_init_first() {
	printf("MarXsCube by seCOnDatkE, 2014-2015.\n\n");
	printf("Initing FSM ...\n");
	
	Generic::Init_Logger();
	
	cube_init_workingdir();
}

void cube_init_workingdir() {
	getcwd(workingdir_buffer, BUFSIZE_WORKINGDIR_PATH);
	Generic::corelog()[L::Debug] << "Working directory: " << workingdir_buffer << rn;
}

void cube_init_atvb() {
	Generic::corelog()[L::Debug] << "Initing AtTheVeryBeginning ..." << rn;
	AtTheVeryBeginning::init_atvb(*Generic::lua_state());
	AtTheVeryBeginning::load_config("atheverybeginning.lua");
}

void cube_init_window() {
	ATVBCube::read_context_setting();
	
	const ATVBCube::CubeInit_WindowSetting *window_setting = &ATVBCube::get_window_setting();
	
	Generic::corelog()[L::Debug] << "Creating window..." << rn;
	window_global = new sf::RenderWindow(sf::VideoMode(window_setting->width, window_setting->height), window_setting->window_title, sf::Style::Titlebar || sf::Style::Close, window_setting->context);
	
	sf::ContextSettings settings_got = window_global->getSettings();
	Generic::corelog()[L::Debug] << "Running with OpenGL " << settings_got.majorVersion << "." << settings_got.minorVersion << rn;
	
	window_global->setFramerateLimit(ATVBCube::get_window_setting().fps_limit);
	window_global->setVerticalSyncEnabled(ATVBCube::get_window_setting().enable_vsync);
}

int main() {

	cube_init_first();
	Generic::Init_LuaStatus();
	Generic::Init_Session();
	cube_init_atvb();

	cube_init_window();
	
	ConfigManger config(*Generic::lua_state());
	LuaInterface::RegisterInterface(*Generic::lua_state());
	LuaUtils::initLuaUtils();
	Generic::Init_FunObjectTableCreate_Forward(config);
	Generic::Init_RenderLayerManger();
	Generic::Init_PhysicsGeneral();
	config.loadConfigFrom("Config.lua");
	
	init_opengl();
	
	Generic::Init_FunObjectTableCreate(config);

	TestManger::GetInstance().window = window_global;
	TestManger::GetInstance().initTest();
	init_terrain_physhape();

	Pathfinding::init(Map::GetInstance().size.x, Map::GetInstance().size.y);
	obsTransform::UpdateVm(0, 0);
	CubeTransform::generate_view_matrix(0, 0);
	
	Acheron::Bullet.start(Acheron::SYNCSTATE::SYNCED);
	Acheron::Silcon.start(Acheron::SYNCSTATE::SYNCED_NONAUTO);
	Acheron::Silcon.invoke();
	
	sf::Event event;
	sf::Clock clock;
	clock_render.restart();
	
	while (window_global->isOpen()) {

		Acheron::Bullet.pause();
		
		global_counter++;
//		float fps = global_counter / clock.getElapsedTime().asSeconds();
//		printf("updating... %lu %f\n", global_counter, fps);
		
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