#pragma once

#include <game_objects/game_object.h>
#include <core/model.h>

class PeanutJar : public GameObject {
public:
	PeanutJar();
	void Init() override;

	void Update(float deltaTime) override;

	void Draw(const SceneParameters& sceneParams) override;

	void ProcessLighting(SceneParameters& sceneParam) override;

private:
	void createShaders();

	void createBody();
	void createCover();
private:
	std::shared_ptr<Shader> _basicUnlitShader{};
	std::shared_ptr<Mesh> _lightMesh{};

	std::vector<Model> _models{};
	std::vector<Texture> _textures;
};