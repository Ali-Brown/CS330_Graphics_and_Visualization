#pragma once

#include <game_objects/game_object.h>
#include <core/model.h>

class Calculator : public GameObject {
public:
	Calculator();
	void Init() override;

	void Update(float deltaTime) override;

	void Draw(const SceneParameters& sceneParams) override;

	void ProcessLighting(SceneParameters& sceneParam) override;

private:
	void createShaders();

	void createBody();
	void createPins();
private:
	std::shared_ptr<Shader> _basicUnlitShader{};
	std::shared_ptr<Mesh> _lightMesh{};

	std::vector<Model> _models{};
	std::vector<Texture> _textures;
};