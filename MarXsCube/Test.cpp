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

#include "FSM.h"
using namespace FSMHelper;
using namespace sf;
using namespace std;

#include "unistd.h"

#include "Atheverybeginning.h"

#include "BulletThread.h"
#include "SilconThread.h"

sf::RenderWindow *window_global = nullptr;

std::size_t global_counter = 0;
std::size_t counter_render = 0;

sf::Clock clock_render;

FSM::FSMLoggerProxy log_main = FSM::create_handle();

void safe_session_close() {
	Acheron::Bullet.invoke_and_stop();
	Acheron::Silcon.invoke_and_stop();
	TestManger::GetInstance().window->close();
}

#include "GLFoundation.h"

int main() {
	printf("MarXsCube by seCOnDatkE, 2014.\n\n");
	
	char currentdir[1024];
    getcwd(currentdir, 1024);
    printf("Working directory: %s, getting ready for logging...\n", currentdir);

	FSM::init();
	FSM::logger(L::Debug) << "test FSM." << rn;
	
	log_main = FSM::logger("main").set_deflogger().get_proxy();
	log_main << "test default logger" << rn;
	
	Generic::Init_Session();

	LuaStatus luaState;
	luaState.init();
	
	AtTheVeryBeginning::init_atvb(luaState);
	AtTheVeryBeginning::load_config("atheverybeginning.lua");
	
	sf::ContextSettings settings;
	settings.stencilBits = 8;
	settings.depthBits = 24;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 2;
	settings.minorVersion = 1;

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Gmap | MarXsCube by seCOnDatkE 2014 - Prototype",
						sf::Style::Titlebar || sf::Style::Close, settings);
	window_global = &window;
	
	sf::ContextSettings settings_got = window.getSettings();
	printf("Running with OpenGL %d.%d.\n", settings_got.majorVersion, settings_got.minorVersion);
	
	ConfigManger config(luaState);
	LuaInterface::RegisterInterface(luaState);
	LuaUtils::initLuaUtils();
	Generic::Init_FunObjectTableCreate_Forward(config);
	Generic::Init_RenderLayerManger();
	Generic::Init_PhysicsGeneral();
	config.loadConfigFrom("Config.lua");
	
	init_opengl();
	
	Generic::Init_FunObjectTableCreate(config);

	TestManger::GetInstance().window = &window;
	window.setFramerateLimit(FPSLimit);
	window.setVerticalSyncEnabled(true);
	TestManger::GetInstance().initTest();

	Pathfinding::init(Map::GetInstance().size.x, Map::GetInstance().size.y);
	obsTransform::UpdateVm(0, 0);
	
	Acheron::Bullet.start(Acheron::SYNCSTATE::SYNCED);
	Acheron::Silcon.start(Acheron::SYNCSTATE::UNSYNCED);
	
	sf::Event event;
	sf::Clock clock;
	clock_render.restart();
	while (window.isOpen()) {

		Acheron::Bullet.pause();
		
		global_counter++;
		float fps = global_counter / clock.getElapsedTime().asSeconds();
//		printf("updating... %lu %f\n", global_counter, fps);
		
		EventManger::GetInstance().GetEvent(EventManger::Events::GAME_UPDATE_BEGIN)();

		Generic::Session()->setMousePos_Ab();
		Generic::Session()->updateMouseButtonStatus();
		
		while (window.pollEvent(event)) {
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
					// Pathfinding::Find(nullptr, nullptr, nullptr);
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
	
	FSM::dispose_logger("main");
	FSM::dispose();

	return 0;
}