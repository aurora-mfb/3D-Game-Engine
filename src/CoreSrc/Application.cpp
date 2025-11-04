
#include "Core/Application.h"
#include <stdexcept>   // std::runtime_error

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Application::Application(int width, int height, const std::string& title)
  : width(width), height(height), title(title), window(nullptr) {
}

bool Application::init() {
	try {
		/* ------------------ INICIALIZACIÓN DE LA VENTANA ----------------*/
		windowManager = std::make_unique<WindowManager>();
		if (!windowManager->init(SCREEN_WIDTH, SCREEN_HEIGHT, title)) {
			throw std::runtime_error("No se pudo inicializar la ventana.");
		}

		window = windowManager->getWindow();

		/* ------------------ INICIALIZACIÓN DEL RENDERER ----------------*/
		renderer = std::make_unique<Renderer>();
		if (!renderer->init()) {
			throw std::runtime_error("Error al inicializar el Renderer (OpenGL/GLEW).");
		}

		/* ------------------ CREACIÓN DEL MUNDO (USANDO WORLDBUILDER) ----------------*/
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

  while (!windowManager->shouldClose() && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
    lastTime = glfwGetTime();

    // Control de cámara
    glm::vec3 pos = camera->getPosition();
    float speed = 5.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      pos += speed * camera->getFront();
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      pos -= speed * camera->getFront();
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
      pos -= speed * camera->getRight();
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
      pos += speed * camera->getRight();

    camera->setPosition(pos);

    // Rotación del modelo
    angle += 32.0f * deltaTime;
    auto entity = world->getEntity(1);
    if (auto model = std::dynamic_pointer_cast<Model>(entity)) {
      model->setRotation(glm::vec3(0.0f, angle, 0.0f));
    }

    // Renderizado
    renderer->clear();
    world->draw();

    windowManager->swapBuffers();
    windowManager->pollEvents();
  }

  windowManager->shutdown();
}

void Application::shutdown() {
  std::cout << "[INFO] Cerrando aplicación..." << std::endl;
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
  std::cout << "[INFO] Recursos liberados correctamente." << std::endl;
}