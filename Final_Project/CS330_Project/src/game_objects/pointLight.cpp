#include <game_objects/pointLight.h>
#include <core/shapes.h>
//#include <core/application.h>
#include <glm/gtc/matrix_transform.hpp>

PointLight::PointLight() {
	createShader();
	createMesh();
}

void PointLight::Init()
{
}

void PointLight::Update(float deltaTime) {
	auto cos = std::cos(totalTime);
	auto sin = std::cos(totalTime);

	//Transform = glm::translate(Transform, glm::vec3(cos/10, 0, sin/10));

	totalTime += deltaTime;
}

void PointLight::Draw(const SceneParameters& sceneParams) {
	for (auto& model : _models) {
		auto* shader = model.GetShader();
		auto* mesh = model.GetMesh();

		//Bind shader program to view, model, and projection matrices
		shader->Bind();
		shader->SetMat4("projection", sceneParams.ProjectionMatrix);
		shader->SetMat4("view", sceneParams.ViewMatrix);
		shader->SetMat4("model", Transform * mesh->Transform);

		mesh->Draw();
	}
}

void PointLight::ProcessLighting(SceneParameters& sceneParams) {
	//get light position from 4th column of light transform
	if (sceneParams.Lights.size() < MAX_POINT_LIGHTS) {
		PointLightStruct pointLight {
			.Position = glm::vec3(Transform[3]),
			.AmbientColor = AmbientColor,
			.DiffuseColor = DiffuseColor,
			.SpecularColor = AmbientColor,
			.Constant = Constant,
			.Linear = Linear,
			.Quadratic = Quadratic
		};
		
		sceneParams.Lights.emplace_back(pointLight);
	}
}

void PointLight::createShader() {
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";
	_basicUnlitShader = std::make_shared<Shader>(shaderPath / "basic_unlit_color.vert", shaderPath / "basic_unlit_color.frag");
}

void PointLight::createMesh() {
	auto cube = std::make_shared<Mesh>(Shapes::cubeVertices, Shapes::cubeElements, glm::vec3(1.f, 1.f, 1.f));
	auto& cubeModel = _models.emplace_back(cube, _basicUnlitShader);
	cubeModel.GetMesh()->Transform = glm::scale(cubeModel.GetMesh()->Transform, glm::vec3(0.1f, 0.1f, 0.1f));
}

