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
#include "Debug.h"
using namespace sf;
using namespace std;

#include "unistd.h"

int main() {
	printf("MarXsCube by seCOnDatkE, 2014.\n\n");
	
    char currentdir[1024];
    getcwd(currentdir, 1024);
    printf("Working directory: %s, getting ready for logging...\n", currentdir);
    
	Debug::createLogFile("log.txt");
	
	Generic::Init_Session();
	Generic::Init_RenderLayerManger();
	Generic::Init_PhysicsGeneral();
	
	LOGFUNC;

	LuaStatus luaState;
	luaState.init();
	LuaUtils::initLuaUtils();
	ConfigManger config(luaState);
	cout << "CubeCore: main - Registering LuaInterface... " << endl;
	LuaInterface::RegisterInterface(luaState);
	Generic::Init_FunObjectTableCreate_Forward(config);
	cout << "CubeCore: main - Loading Config... " << endl;
	config.loadConfigFrom("Config.lua");
	
	Generic::Init_FunObjectTableCreate(config);
	
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Gmap | MarXsCube by seCOnDatkE 2014 - Prototype",
						sf::Style::Titlebar || sf::Style::Close, settings);

	TestManger::GetInstance().window = &window;
	window.setFramerateLimit(FPSLimit);
	TestManger::GetInstance().initTest();

	Pathfinding::init(Map::GetInstance().size.x, Map::GetInstance().size.y);
	obsTransform::UpdateVm(0, 0);

	sf::Event event;
	while (window.isOpen()) {
		EventManger::GetInstance().GetEvent(EventManger::Events::GAME_UPDATE_BEGIN)();
		// printf("CubeCore: main - updating ...\n");

		Generic::Session()->setMousePos_Ab();
		Generic::Session()->updateMouseButtonStatus();
		while (window.pollEvent(event)) {
			switch (event.type) {
				case Event::Closed: window.close(); break;

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

		window.clear(sf::Color::Black);

		// printf("CubeCore: main - updating logic ...\n");
		for (auto i : ObjectManger::GetInstance().Objects)
			i->Update();

		// printf("CubeCore: main - updating render ...\n");
		for (size_t i = 0; i < RenderLayerType::Count; i++)
			Generic::RenderLayerManger()->Layers[i].Update();

		ObjectManger::GetInstance().FinishRemove();

		// printf("CubeCore: main - updating physics ...\n");
		Generic::PhysicsGeneral()->dynaWorld->stepSimulation(1.f/(float)FPSLimit, 10, btScalar(1.)/btScalar((float)divPhysics));

		// printf("CubeCore: main - displaying ...\n");
		window.display();
	}
	Generic::Dispose_PhysicsGeneral();
	
	Debug::closeLogFile();

	return 0;
}