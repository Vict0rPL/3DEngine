
// main.cpp
#include "Engine.h"
int main(int argc, char** argv) { 
	Engine engine(argc, argv); 
	Engine::instance = &engine; 
	engine.SetResolution(800, 600); 
	engine.SetClearColor(0.1f, 0.1f, 0.2f); 
	engine.SetFullscreen(false); 
	engine.SetFPS(60); 
	engine.SetPerspective(60.0f, 0.1f, 100.0f); 
	engine.Init(); 
	engine.Run(); 
	engine.Cleanup(); 
	return 0; 
}
