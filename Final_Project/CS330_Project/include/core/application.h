#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <mesh.h>
#include <shader.h>
#include <camera.h>
#include <texture.h>
#include <game_objects/game_object.h>

class Application {
public:
	Application(std::string WindowTitle, int width, int height);
	void Run();

private:
	bool openWindow();
	void setupInputs();

	void setUpScene();
	bool update(float deltaTime);
	bool draw();

	void handleInput(float deltaTime);
	void mousePositionCallback(double xpos, double ypos);

private:
	std::string _applicationName{};
	int _width{};
	int _height{};
	GLFWwindow* _window{ nullptr };

	float _moveSpeed{ .5f };
	Camera _camera;
	std::vector<Mesh> _meshes;
	std::vector<std::unique_ptr<GameObject>> _objects{};
	std::vector<Texture> _textures;
	Shader _shader;
	Shader _basicLitShader;
	bool _running{ false };

	bool _firstMouse{ false };
	glm::vec2 _lastMousePosition{-1, -1};
	glm::vec2 _cameraLookSpeed{};

	float _lastFrameTime{-1.5f};

	GLuint _containerTexture;
	GLuint _smileTexture;

	//lighting variables
	float _ambientStrength{ 0.1f };
	glm::vec3 _ambientLightColor{1.f, 1.f, 1.f};
};