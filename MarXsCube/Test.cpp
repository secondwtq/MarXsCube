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
#include "Debug.h"
using namespace sf;
using namespace std;

#include "unistd.h"

int main() {
	printf("MarXsCube by seCOnDatkE, 2014.");
	
    char currentdir[1024];
    getcwd(currentdir, 1024);
    printf("Working directory: %s, getting ready for logging...\n", currentdir);
    
	Debug::createLogFile("log.txt");
	LOGFUNC;

	LuaStatus luaState;
	luaState.init();
	LuaUtils::initLuaUtils();
	ConfigManger config(luaState);
	cout << "CubeCore: main - Registering LuaInterface... " << endl;
	LuaInterface::RegisterInterface(luaState);
	cout << "CubeCore: main - Loading Config... " << endl;
	config.loadConfigFrom("Config.lua");

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TestWindow | MarXsCube by seCOnDatkE 2014 - Prototype",
						sf::Style::Titlebar || sf::Style::Close);

	TestManger::GetInstance().window = &window;
	window.setFramerateLimit(FPSLimit);
	TestManger::GetInstance().initTest();

	Session::GetInstance().initSession();
	Pathfinding::init(Map::GetInstance().size.x, Map::GetInstance().size.y);
	UpdateVm(0, 0);

	sf::Event event;
	while (window.isOpen()) {
		EventManger::GetInstance().GetEvent(EventManger::Events::GAME_UPDATE_BEGIN)();
		// printf("CubeCore: main - updating ...\n");

		Session::GetInstance().setMousePos_Ab();
		Session::GetInstance().updateMouseButtonStatus();
		while (window.pollEvent(event)) {
			switch (event.type) {
				case Event::Closed: window.close(); break;

				case Event::KeyPressed:
					Session::GetInstance().setKeyEvent(event.key);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_KEYPRESS)(Session::GetInstance().getKeyEvent());
					break;

				case Event::KeyReleased:
					Session::GetInstance().setKeyEvent(event.key);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_KEYRELEASE)(Session::GetInstance().getKeyEvent());
					break;

				case Event::MouseButtonPressed:
					Session::GetInstance().setMousePos_Press(event.mouseButton);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_MOUSEPRESS)(Session::GetInstance().getMousePos());
					// Pathfinding::Find(nullptr, nullptr, nullptr);
					break;

				case Event::MouseButtonReleased:
					Session::GetInstance().setMousePos_Release(event.mouseButton);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_MOUSERELEASE)(Session::GetInstance().getMousePos());
					break;

				case Event::MouseMoved:
					Session::GetInstance().setMousePos_Rl(event.mouseMove);
					EventManger::GetInstance().GetEvent(EventManger::Events::UI_MOUSEMOVE)(Session::GetInstance().getMousePos());
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
			RenderLayerManger::GetInstance().Layers[i].Update();

		ObjectManger::GetInstance().FinishRemove();

		// printf("CubeCore: main - updating physics ...\n");
		dynaWorld->stepSimulation(1.f/(float)FPSLimit, 10, btScalar(1.)/btScalar((float)divPhysics));

		// printf("CubeCore: main - displaying ...\n");
		window.display();
	}
	Debug::closeLogFile();

	return 0;
}