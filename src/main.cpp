
#include "Core/Application.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() 
{
	// Create an instance of the Application with the given width, height, and window title.
	Application app(SCREEN_WIDTH, SCREEN_HEIGHT, "Ugine3D"); 

	// Initialize the application: this sets up the window, OpenGL, and the scene.
	if (!app.init()) 
	{ 
    std::cerr << "Error initializing the application." << std::endl;
		return -1; // Return an error code if initialization fails.
	}

	// Run the main loop of the application.
	// This will handle rendering, input, and updates until the window is closed.
  app.run();

	// Return 0 to indicate the program terminates successfully;
  return 0;
}