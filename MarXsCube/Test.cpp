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

#include "ModelLoader_obj.h"
gl_vertarray verts_def;
gl_shader tiler_shader_main;
GLIDX texture_main = 0;

int vert_attrid = 0;
int vert_normid = 0;
int vert_textid = 0;
int vert_texcid = 0;

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

#include <SFML/OpenGL.hpp>

static const GLfloat g_vertex_bufdata[] = {
	-1, 1, 0, 0, 1,
	-1, -1, 0, 1, 0,
	1, -1, 0, 1, 1,
	
	1, -1, 0, 1, 1,
	1, 1, 0, 1, 1,
	-1, 1, 0, 0, 1,
};

GLuint vert_buf;

void init_opengl() {
	window_global->setActive();
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glDisable(GL_LIGHTING);
	
	glViewport(0, 0, window_global->getSize().x, window_global->getSize().y);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio = static_cast<float>(window_global->getSize().x) / window_global->getSize().y;
	glOrtho(-1.f, 1.f, -1.f/ratio, 1.f/ratio, .1f, 500.f);
	
	tiler_shader_main.load_file(gl_shader::type::SHADER_VERTEX, "terrain_tiler.vert");
	tiler_shader_main.load_file(gl_shader::type::SHADER_FRAG, "terrain_tiler.frag");
	tiler_shader_main.create();
	
	vert_attrid = tiler_shader_main.get_attribute("position");
	vert_normid = tiler_shader_main.get_attribute("s_normal");
	vert_textid = tiler_shader_main.get_attribute("s_texture_main");
	vert_texcid = tiler_shader_main.get_attribute("s_texcoord");
	
	texture_main = TextureManger::GetInstance().TextureHashs["DOGE"]->texture.m_texture;
	
	std::cout << tiler_shader_main.log(gl_shader::type::SHADER_VERTEX);
	std::cout << tiler_shader_main.log(gl_shader::type::SHADER_FRAG);
	
//	glDisableClientState(GL_NORMAL_ARRAY);
//	glDisableClientState(GL_COLOR_ARRAY);
}

void render_gl() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(6, 6, 5, 0, 0, 0, 0, 0, 1);
	glScalef(0.1, 0.1, 0.1);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	tiler_shader_main.use();
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
//	glVertexPointer(3, GL_FLOAT, 5*sizeof(GLfloat), (char *)0);
	glVertexAttribPointer(vert_attrid, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (char *)0);
	glVertexAttribPointer(vert_normid, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (char *)(3*sizeof(GLfloat)));
	glVertexAttribPointer(vert_texcid, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (char *)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(vert_attrid);
	glEnableVertexAttribArray(vert_normid);
	glEnableVertexAttribArray(vert_texcid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_main);
	glUniform1i(vert_textid, 0);
	glDrawArrays(GL_TRIANGLES, 0, (int)verts_def.len());
	glDisableVertexAttribArray(vert_texcid);
	glDisableVertexAttribArray(vert_normid);
	glDisableVertexAttribArray(vert_attrid);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void thread_rendering() {
	std::unique_lock<std::mutex> lk(mut_render);
	
	update_render.wait(lk, [] { return should_render; });
	
	window_global->setActive();
	while (true) {
		if (!game_running) return lk.unlock();
		
		counter_render++;
		float fps = counter_render / clock_render.getElapsedTime().asSeconds();
		printf("rendering... %lu %f\n", counter_render, fps);
		window_global->clear(sf::Color::Black);
		
		window_global->pushGLStates();
		for (size_t i = 0; i < RenderLayerType::Count; i++)
			Generic::RenderLayerManger()->Layers[i].Update();
		window_global->popGLStates();
		
		render_gl();
		
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
	settings.stencilBits = 8;
	settings.depthBits = 24;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 2;
	settings.minorVersion = 1;

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Gmap | MarXsCube by seCOnDatkE 2014 - Prototype",
						sf::Style::Titlebar || sf::Style::Close, settings);
	window_global = &window;
	
	sf::ContextSettings settings_got = window.getSettings();
	printf("Running with OpenGL %d.%d.\n", settings_got.majorVersion, settings_got.minorVersion);
	
	objfile obj_test;
	obj_test.filepath = "drawcall.obj";
	obj_test.parse();
	
	transfer_verts(verts_def, obj_test);
	
	init_opengl();
	
	glGenBuffers(1, &vert_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
	glBufferData(GL_ARRAY_BUFFER, verts_def.len() * sizeof(float), verts_def.array(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	TestManger::GetInstance().window = &window;
	window.setFramerateLimit(FPSLimit);
	window.setVerticalSyncEnabled(true);
	TestManger::GetInstance().initTest();

	Pathfinding::init(Map::GetInstance().size.x, Map::GetInstance().size.y);
	obsTransform::UpdateVm(0, 0);

	game_running = true;
	t_thr_phy = std::thread(thread_physics);
	t_thr_ren = std::thread(thread_rendering);
	
	sf::Event event;
	sf::Clock clock;
	clock_render.restart();
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