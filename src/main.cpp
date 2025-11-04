#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "Core/Application.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() 
{
	Application app(SCREEN_WIDTH, SCREEN_HEIGHT, "Ugine3D"); 
	if (!app.init()) 
	{ 
    std::cerr << "Error al inicializar la aplicación." << std::endl;
		return -1; 
	}

  app.run();

  return 0;
}