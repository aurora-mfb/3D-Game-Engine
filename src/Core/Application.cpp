
#include "Core/Application.h"
#include <stdexcept>   // std::runtime_error

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Application::Application(int width, int height, const std::string& title)
  : width(width), height(height), title(title), window(nullptr) {
}

bool Application::init() {
	/* ------------------ INICIALIZACIÓN DE LA VENTANA Y DE GLFW ----------------*/
	// Init glfw, inicializar la librería GLFW
	if (!glfwInit()) {
		std::cout << "could not initialize glfw" << std::endl;
		return -1;
	}

	// Create window, solicita que el contexto tenga multisampling con 8 muestras para antialiasing (suavizar bordes).
	//glfwWindowHint(GLFW_RESIZABLE, false); // si se descomenta, la ventana no se podría redimensionar.
	glfwWindowHint(GLFW_SAMPLES, 8);

	// Crea una ventana con las dimensiones indicadas.
	GLFWwindow* win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "", nullptr, nullptr);
	if (!win) {
		std::cout << "could not create opengl window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Hace que el contexto OpenGL de la ventana recién creada sea el actual para que las llamadas OpenGL afecten a esta ventana.
	glfwMakeContextCurrent(win);

	// Inicialización del motor
	if (!initGL()) {
		std::cerr << "Error en la inicialización de OpenGL" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(win, GLFW_STICKY_KEYS, GLFW_TRUE);

	/* ------------------ CREACIÓN DEL SHADER ----------------*/
	// Creación de los shaders
	// Cargando, compilando y validando un shader que será usado como el shader por defecto en tu motor gráfico.
	State::defaultShader = std::make_shared<Shader>("data/vertex.glsl", "data/fragment.glsl");
	if (State::defaultShader->getId() == 0) {
		std::cerr << "Error al crear el shader: " << State::defaultShader->getError() << std::endl;
		glfwTerminate();
		return -1;
	}

	/* ------------------ INICIALIZACIÓN STATE ----------------*/

	// Establecer la matriz de proyeccio y vista en el estado global del motor gráfico.
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	State::projectionMatrix = projection;
	State::viewMatrix = glm::mat4(1.0f);  // Vista inicial (sin transformación)


	/* ------------------ CREACIÓN DE LA MALLA ----------------*/
	// Se construye la malla que representa parte de un cubo 3d texturizado, usando dos caras distintas con dos texturas diferentes.

	// Crea un nuevo objeto Mesh (malla), que contendrá múltiples buffers + materiales (uno por cara del cubo).
	std::shared_ptr<Mesh> cowboyMesh = Mesh::load("data/Models/gunslinger.obj", State::defaultShader);
	if (!cowboyMesh) {
		std::cerr << "Error cargando el modelo OBJ" << std::endl;
		glfwTerminate();
		return -1;
	}

	//std::shared_ptr<Mesh> boxMesh = Mesh::load("data/Models/box_stack.obj", State::defaultShader);
	//if (!boxMesh) {
	//	std::cerr << "Error cargando el modelo OBJ" << std::endl;
	//	glfwTerminate();
	//	return -1;
	//}

	/* ------------------ CREACIÓN DE LA CAMARA ----------------*/
	// Crear e inicializar una cámara 3D (Camera) que se usará para renderizar la escena en OpenGL.

	glm::ivec4 viewport = glm::ivec4(0, 0, 800, 600);  // Tamaño de la ventana
	glm::vec3 clearColor = glm::vec3(0.1f, 0.1f, 0.1f);  // Color de fondo
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(projection, viewport, clearColor);
	camera->setPosition(glm::vec3(0.0f, 1.0f, -20.0f));
	camera->setRotation(glm::vec3(glm::radians(0.0f), 0.0f, 0.0f));

	/* ------------------ CREACIÓN DEL MUNDO ----------------*/
	World world;
	world.addEntity(camera);

	/* ------------------ CREACIÓN DEL MODELO ----------------*/
	// Creación del modelo del cubo y adición al mundo.

	Model cowboyModel(cowboyMesh);
	cowboyModel.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	world.addEntity(std::make_shared<Model>(cowboyModel));

	//Model boxModel(boxMesh);
	//boxModel.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//world.addEntity(std::make_shared<Model>(boxModel));

	/* ------------------ LOOP PRINCIPAL ----------------*/
	// main loop
	float angle = 0;
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(win) && !glfwGetKey(win, GLFW_KEY_ESCAPE)) {

		// get delta time
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		// get window size
		int screenWidth, screenHeight;
		glfwGetWindowSize(win, &screenWidth, &screenHeight);
		glm::ivec4 viewport(0, 0, screenWidth, screenHeight);
		camera->setViewport(viewport);

		glm::vec3 pos = camera->getPosition();
		float speed = 5.0f * deltaTime;

		if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)
			pos += speed * camera->getFront();
		if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)
			pos -= speed * camera->getFront();
		if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS)
			pos -= speed * camera->getRight();
		if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS)
			pos += speed * camera->getRight();

		camera->setPosition(pos);

		angle += 32.0f * deltaTime;

		std::shared_ptr<Entity> entity = world.getEntity(1);
		std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(entity);
		if (model)
		{
			model->setRotation(glm::vec3(0.0f, angle, 0.0f));
		}
		else
		{
			std::cout << "La entidad no es un modelo." << std::endl;
		}

		// Se llama a la función que dibuja todas las entidades del mundo.
		world.draw();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// shutdown
	glfwTerminate();
}

void Application::run() {
	std::cout << "[DEBUG] Contexto actual en run(): " << glfwGetCurrentContext() << std::endl;

  if (!window) {
    std::cerr << "[ERROR] No se puede ejecutar la aplicación sin una ventana válida." << std::endl;
    return;
  }

	float angle = 0;
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

		// get delta time
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		// get window size
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		glm::ivec4 viewport(0, 0, screenWidth, screenHeight);
		camera->setViewport(viewport);

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

		angle += 32.0f * deltaTime;

		std::shared_ptr<Entity> entity = world->getEntity(1);
		std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(entity);
		if (model)
		{
			model->setRotation(glm::vec3(0.0f, angle, 0.0f));
		}
		else
		{
			std::cout << "La entidad no es un modelo." << std::endl;
		}

		// Se llama a la funci?n que dibuja todas las entidades del mundo.
		std::cout << "[DEBUG] Shader ID: " << State::defaultShader->getId() << std::endl;
		world->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// shutdown
	glfwTerminate();
}

bool Application::initGL() {
	// Inicializar GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Error al inicializar GLEW: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	// Activar GL_DEPTH_TEST, la prueba de profundidad.
	// Esto permite que OpenGL controle qué fragmentos (píxeles) están más cerca de la cámara y deben ser visibles, y 
	// cuáles están detrás de otros y deben ocultarse.
	glEnable(GL_DEPTH_TEST);

	// Activar GL_SCISSOR_TEST, el test de tijera (scissor test).
	// Esto permite limitar el área de dibujo a una región rectangular específica en la ventana.
	glEnable(GL_SCISSOR_TEST);

	return true;
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