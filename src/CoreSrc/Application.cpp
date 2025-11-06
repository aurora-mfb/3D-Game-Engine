#include "Core/Application.h"

Application::Application(int width, int height, const std::string& title)
  : width(width), height(height), title(title), window(nullptr) {
}

bool Application::init() {
	try {
    /* ------------------ WINDOW INITIALIZATION ------------------ */
		windowManager = std::make_unique<WindowManager>();
		if (!windowManager->init(width, height, title)) {
			throw std::runtime_error("Failed to initialize the window.");
		}

		window = windowManager->getWindow();

    /* ------------------ RENDERER INITIALIZATION ------------------ */
		renderer = std::make_unique<Renderer>();
		if (!renderer->init()) {
			throw std::runtime_error("Failed to initialize the Renderer(OpenGL / GLEW).");
		}

    /* ------------------ WORLD CREATION (VIA WORLDBUILDER) ------------------ */
    // The WorldBuilder creates a default world with a shader, camera, and a 3D model.
		world = WorldBuilder::createDefaultWorld(State::defaultShader, camera);

		return true;
	}
	catch (const std::runtime_error& e) {
		std::cerr << "[ERROR] " << e.what() << std::endl;
		if (windowManager) windowManager->shutdown();
		return false;
	}
}

void Application::run() {
  float angle = 0.0f;
  double lastTime = glfwGetTime();

  input = std::make_shared<InputManager>();

  while (!windowManager->shouldClose() && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    // Calculate time elapsed between frames
    float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
    lastTime = glfwGetTime();

    /* ------------------ CAMERA VIEWPORT CHANGES ------------------ */
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glm::ivec4 viewport = glm::ivec4(vp[0], vp[1], vp[2], vp[3]);

    camera->setViewport(viewport);

    /* ------------------ CAMERA MOVEMENT ------------------ */
    input->handleInput(window, camera, deltaTime);

    /* ------------------ MODEL ROTATION ------------------ */
    angle += 32.0f * deltaTime;
    auto entity = world->getEntity(1);
    if (auto model = std::dynamic_pointer_cast<Model>(entity)) {
      model->setRotation(glm::vec3(0.0f, angle, 0.0f));
    }

    /* ------------------ RENDERING ------------------ */
    world->draw(); // Draw all entities in the world

    windowManager->swapBuffers(); // Display the rendered frame
    windowManager->pollEvents(); // Process input and OS events
  }

  windowManager->shutdown();
}

void Application::shutdown() {
  std::cout << "[INFO] Closing application..." << std::endl;
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
  std::cout << "[INFO] Resources released successfully." << std::endl;
}