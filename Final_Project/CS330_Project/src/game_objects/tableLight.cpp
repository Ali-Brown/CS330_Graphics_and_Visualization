#include <game_objects/tableLight.h>
#include <core/shapes.h>
#include <rendering/types.h>
#include <glm/gtc/matrix_transform.hpp>

TableLight::TableLight() {
	createShaders();
	createBody();
	createBase();
	createVisor();
}

void TableLight::Init() {
}

void TableLight::Update(float deltaTime) {
	//Transform = glm::rotate(Transform, glm::radians(45.f) * deltaTime, glm::vec3(0, 1, 0));
}

void TableLight::Draw(const SceneParameters& sceneParams) {
	Shader* lastBoundShader = nullptr;

	for (auto& model : _models) {
		auto* shader = model.GetShader();
		auto* mesh = model.GetMesh();

		//Bind shader program to view, model, and projection matrices
		if (shader != lastBoundShader) {
			shader->Bind();
			shader->SetMat4("projection", sceneParams.ProjectionMatrix);
			shader->SetMat4("view", sceneParams.ViewMatrix);

			// Set camera position
			shader->SetVec3("eyePos", sceneParams.CameraPosition);

			//handle lights
			for (auto i = 0; i < MAX_POINT_LIGHTS; i++) {
				std::string baseUniformName = "pointLights[";
				baseUniformName += std::to_string(i) + "]";

				PointLightStruct pointLight = i < sceneParams.Lights.size() ? sceneParams.Lights[i] : PointLightStruct{};

				shader->SetVec3(baseUniformName + ".Position", pointLight.Position);
				shader->SetVec3(baseUniformName + ".DiffuseColor", pointLight.DiffuseColor);
				shader->SetVec3(baseUniformName + ".AmbientColor", pointLight.AmbientColor);
				shader->SetVec3(baseUniformName + ".SpecularColor", pointLight.SpecularColor);

				shader->SetFloat(baseUniformName += ".Constant", pointLight.Constant);
				shader->SetFloat(baseUniformName += ".Linear", pointLight.Linear);
				shader->SetFloat(baseUniformName += ".Quadratic", pointLight.Quadratic);
			}

			shader->SetVec3("dirLight.Direction", sceneParams.DirLight.Direction);
			shader->SetVec3("dirLight.AmbientColor", sceneParams.DirLight.AmbientColor);
			shader->SetVec3("dirLight.DiffuseColor", sceneParams.DirLight.DiffuseColor);
			shader->SetVec3("dirLight.SpecularColor", sceneParams.DirLight.SpecularColor);
		}

		// Bind shader program to view, model, and projection matrices, and textures
		shader->SetInt("tex0", 0);
		shader->SetInt("tex1", 1);

		//Bind shader program to textures 
		for (auto i = 0; i < _textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);

			_textures[i].Bind();
		}

		shader->SetMat4("model", Transform * mesh->Transform);
		mesh->Draw();
	}
}


void TableLight::ProcessLighting(SceneParameters& sceneParams) {
	//Not a light, Do do nothing
	return;
}

void TableLight::createShaders() {
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";
	_basicUnlitShader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");

	//Load cube mesh testures
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";
	_textures.emplace_back(texturePath / "plastic2.jpg");
	_textures.emplace_back(texturePath / "plastic2.jpg");
}

void TableLight::createBase() {
	auto cube = std::make_shared<Mesh>(Shapes::cubeVertices, Shapes::cubeElements, glm::vec3(0.6f, 0.42f, 0.12f));
	auto& lightBase = _models.emplace_back(cube, _basicUnlitShader);
	//lightBody.GetMesh()->Transform = glm::translate(lightBody.GetMesh()->Transform, glm::vec3(0.f, 0.f, 1.f));
	lightBase.GetMesh()->Transform = glm::scale(lightBase.GetMesh()->Transform, glm::vec3(0.46f, 0.015f, 0.36f));
}


void TableLight::createBody() {
	auto cube = std::make_shared<Mesh>(Shapes::cubeVertices, Shapes::cubeElements, glm::vec3(0.6f, 0.42f, 0.12f));
	auto& lightBody = _models.emplace_back(cube, _basicUnlitShader);
	lightBody.GetMesh()->Transform = glm::translate(lightBody.GetMesh()->Transform, glm::vec3(0.f, 0.175f, 0.1f));
	lightBody.GetMesh()->Transform = glm::rotate(lightBody.GetMesh()->Transform, glm::radians(65.f), glm::vec3(1, 0, 0));
	lightBody.GetMesh()->Transform = glm::scale(lightBody.GetMesh()->Transform, glm::vec3(0.46f, 0.08f, 0.36f));
}

void TableLight::createVisor() {
	auto [cylinderVertices, cylinderIndices] = Shapes::BuildCylinderSmooth(32, 0.1f, 0.15f);
	auto lightVisor = std::make_shared<Mesh>(cylinderVertices, cylinderIndices, glm::vec3(0.6f, 0.42f, 0.12f));
	_models.emplace_back(lightVisor, _basicUnlitShader);

	lightVisor->Transform = glm::translate(lightVisor->Transform, glm::vec3(0.f, 0.175f, 0.15f));
	lightVisor->Transform = glm::rotate(lightVisor->Transform, glm::radians(-25.f), glm::vec3(1, 0, 0));
}
