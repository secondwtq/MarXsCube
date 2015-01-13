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

#include <thread>
#include <mutex>
#include <condition_variable>

bool game_running = false;
bool should_update = false;
std::mutex mut_phy;
std::condition_variable update_phy;

bool should_render = false;
std::mutex mut_render;
std::condition_variable update_render;

sf::RenderWindow *window_global = nullptr;

std::size_t global_counter = 0;
std::size_t counter_render = 0;

sf::Clock clock_render;

std::thread t_thr_phy;
std::thread t_thr_ren;

void safe_session_close() {
	game_running = false;
	t_thr_ren.join();
	TestManger::GetInstance().window->close();
}

void thread_physics() {
	while (true) {
		std::unique_lock<std::mutex> lk(mut_phy);
		update_phy.wait(lk, [] { return should_update; });
		if (!game_running) return lk.unlock();
		Generic::PhysicsGeneral()->dynaWorld->stepSimulation(1.f/(float)FPSLimit, 16, btScalar(1.)/btScalar((float)divPhysics));
		should_update = false;
		lk.unlock();
	}
}

void thread_rendering() {
	std::unique_lock<std::mutex> lk(mut_render);
	update_render.wait(lk, [] { return should_render; });
	while (true) {
		if (!game_running) return lk.unlock();
		
		counter_render++;
		float fps = counter_render / clock_render.getElapsedTime().asSeconds();
		printf("rendering... %lu %f\n", counter_render, fps);
		window_global->clear(sf::Color::Black);
		for (size_t i = 0; i < RenderLayerType::Count; i++)
			Generic::RenderLayerManger()->Layers[i].Update();
//		should_render = false;
		window_global->display();
//		lk.unlock();
	}
}

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
	window_global = &window;

	TestManger::GetInstance().window = &window;
	window.setFramerateLimit(FPSLimit);
	window.setVerticalSyncEnabled(false);
	TestManger::GetInstance().initTest();

	Pathfinding::init(Map::GetInstance().size.x, Map::GetInstance().size.y);
	obsTransform::UpdateVm(0, 0);

	game_running = true;
	t_thr_phy = std::thread(thread_physics);
	t_thr_ren = std::thread(thread_rendering);
	
	sf::Event event;
	sf::Clock clock;
	while (window.isOpen()) {
//		mut_render.lock();
		mut_phy.lock();
		
		global_counter++;
		float fps = global_counter / clock.getElapsedTime().asSeconds();
		printf("updating... %lu %f\n", global_counter, fps);
		
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
		
		mut_phy.unlock();
//		mut_render.unlock();
		
		should_update = true;
		should_render = true;
		update_phy.notify_all();
		update_render.notify_all();
		
//		printf("updating complete %lu %f\n", global_counter, fps);
	}
	
	should_update = true;
	should_render = true;
	update_phy.notify_all();
	update_render.notify_all();
	t_thr_phy.join();
	
	Generic::Dispose_PhysicsGeneral();
	
	Debug::closeLogFile();

	return 0;
}