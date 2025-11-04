# 3D-Game-Engine
Description:
This project is a 3D game engine developed entirely from scratch in C++ using OpenGL. It was created as a final project for a computer graphics class in U-Tad, focusing on learning and implementing the core systems of a 3D engine.

<img width="693" height="566" alt="image" src="https://github.com/user-attachments/assets/f869b427-982f-4a91-8bef-a16cd4e27cd0" />

## Key Features:
* 3D Rendering: Implements a rendering pipeline using OpenGL, including support for shaders, meshes, materials, textures, and transformations.
* Scene Management: Organizes entities and models into a scene graph (World class) for structured rendering and updates.
* Camera System: Implements a 3D camera with position, rotation, and movement controls.
* Input Handling: Processes keyboard input for camera movement and interactions using a dedicated InputManager.
* Window & Context Management: Uses WindowManager to handle window creation, GLFW initialization, and OpenGL context setup.
* Modular Architecture: Follows the principle of single responsibility, with classes like Renderer, WorldBuilder, and InputManager handling separate aspects of the engine.
* OBJ Model Loading: Supports loading .obj 3D models and rendering them with textures and shaders.
* Shader Management: Handles GLSL shaders, including compilation, error checking, and binding.

## Purpose:
The goal of this project was to gain hands-on experience with low-level graphics programming, understand how 3D engines manage rendering pipelines, input, and scene organization, and develop a foundational engine that can be extended for games or graphics applications.

## Technologies Used:
* C++17 – Core language for engine logic
* OpenGL 3.3+ – Rendering API for 3D graphics
* GLEW – OpenGL extension loader
* GLFW – Window creation and input handling
* GLM – Math library for vectors, matrices, and transformations

## Project Structure:
* Core/ – Main application loop and engine initialization (Application, States, etc.)
* Managers/ – Subsystems like WindowManager, InputManager, WorldBuilder or Renderer.
* Graphics/ – Meshes, shaders, buffers, materials, and textures.
* Scene/ – 3D model assets, world, entities, etc.
* Data/ – Shaders, textures, and other resources

## How to Run:
* Compile the project using a C++17 compiler with OpenGL, GLFW, and GLEW libraries, in my case, I used Visual Studio.
* Run the executable. A window will open showing a 3D model that can be rotated and navigated using the arrow keys.

## Learning Outcomes:
* Deep understanding of 3D rendering pipelines and graphics APIs.
* Experience building modular game engine systems from scratch.
* Hands-on practice with shader programming, model loading, and scene management.
