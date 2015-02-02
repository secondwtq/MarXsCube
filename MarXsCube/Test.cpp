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

struct CubeInit_WindowSetting {
public:
	sf::ContextSettings context;
	
	std::string window_title;
	
	unsigned int width, height;
};

CubeInit_WindowSetting cube_read_context_setting() {
	Generic::corelog()[L::Debug] << "Reading window setting..." << rn;
	
	CubeInit_WindowSetting ret;
	sf::ContextSettings ret_context;
	ret_context.majorVersion = AtTheVeryBeginning::getatvb<unsigned int>("context_version_major");
	ret_context.minorVersion = AtTheVeryBeginning::getatvb<unsigned int>("context_version_minor");
	ret_context.antialiasingLevel = AtTheVeryBeginning::getatvb<unsigned int>("context_antialias");
	ret_context.stencilBits = 8;
	ret_context.depthBits = 24;
	
	ret.context = ret_context;
	ret.window_title = AtTheVeryBeginning::getatvb<std::string>("window_title") + " | MarXsCube Prototype";
	ret.width = AtTheVeryBeginning::getatvb<unsigned int>("width");
	ret.height = AtTheVeryBeginning::getatvb<unsigned int>("height");
	return ret;
}

void cube_init_window() {
	CubeInit_WindowSetting window_setting = cube_read_context_setting();
	
	Generic::corelog()[L::Debug] << "Creating window..." << rn;
	window_global = new sf::RenderWindow(sf::VideoMode(window_setting.width, window_setting.height), window_setting.window_title, sf::Style::Titlebar || sf::Style::Close, window_setting.context);
	
	sf::ContextSettings settings_got = window_global->getSettings();
	Generic::corelog()[L::Debug] << "Running with OpenGL " << settings_got.majorVersion << "." << settings_got.minorVersion << rn;
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
	window_global->setFramerateLimit(FPSLimit);
	window_global->setVerticalSyncEnabled(true);
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