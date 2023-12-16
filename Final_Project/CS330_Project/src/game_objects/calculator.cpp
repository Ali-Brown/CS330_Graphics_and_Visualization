#include <game_objects/calculator.h>
#include <core/shapes.h>
#include <rendering/types.h>
#include <glm/gtc/matrix_transform.hpp>

Calculator::Calculator() {
	createShaders();
	createBody();
	createPins();
}

void Calculator::Init() {
}

void Calculator::Update(float deltaTime) {
	//Transform = glm::rotate(Transform, glm::radians(45.f) * deltaTime, glm::vec3(0, 1, 0));
}

void Calculator::Draw(const SceneParameters& sceneParams) {
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


void Calculator::ProcessLighting(SceneParameters& sceneParams) {
	//Not a light, Do do nothing
	return;
}

void Calculator::createShaders() {
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";
	_basicUnlitShader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");

	//Load cube mesh testures
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";
	_textures.emplace_back(texturePath / "plastic1.jpg");
	_textures.emplace_back(texturePath / "plastic1.jpg");
}

void Calculator::createPins() {
	auto [cylinderVertices, cylinderIndices] = Shapes::BuildCylinderSmooth(32, 0.025f, 0.05f);
	auto pin1 = std::make_shared<Mesh>(cylinderVertices, cylinderIndices, glm::vec3(0.2f, 0.2f, 0.2f));
	auto pin2 = std::make_shared<Mesh>(cylinderVertices, cylinderIndices, glm::vec3(0.2f, 0.2f, 0.2f));
	auto pin3 = std::make_shared<Mesh>(cylinderVertices, cylinderIndices, glm::vec3(0.2f, 0.2f, 0.2f));
	auto pin4 = std::make_shared<Mesh>(cylinderVertices, cylinderIndices, glm::vec3(0.2f, 0.2f, 0.2f));
	
	_models.emplace_back(pin1, _basicUnlitShader);
	_models.emplace_back(pin2, _basicUnlitShader);
	_models.emplace_back(pin3, _basicUnlitShader);
	_models.emplace_back(pin4, _basicUnlitShader);

	pin1->Transform = glm::translate(pin1->Transform, glm::vec3(-0.2f, 0.21f, 0.55f));
	pin1->Transform = glm::rotate(pin1->Transform, glm::radians(-90.f), glm::vec3(1, 0, 0));

	pin2->Transform = glm::translate(pin2->Transform, glm::vec3(0.2f, 0.21f, 0.55f));
	pin2->Transform = glm::rotate(pin2->Transform, glm::radians(-90.f), glm::vec3(1, 0, 0));

	pin3->Transform = glm::translate(pin3->Transform, glm::vec3(-0.2f, 0.21f, -0.35f));
	pin3->Transform = glm::rotate(pin3->Transform, glm::radians(-90.f), glm::vec3(1, 0, 0));

	pin4->Transform = glm::translate(pin4->Transform, glm::vec3(0.2f, 0.21f, -0.35f));
	pin4->Transform = glm::rotate(pin4->Transform, glm::radians(-90.f), glm::vec3(1, 0, 0));
}


void Calculator::createBody() {
	auto cube = std::make_shared<Mesh>(Shapes::cubeVertices, Shapes::cubeElements, glm::vec3(0.2f, 0.2f, 0.2f));
	auto& lightBody = _models.emplace_back(cube, _basicUnlitShader);
	lightBody.GetMesh()->Transform = glm::translate(lightBody.GetMesh()->Transform, glm::vec3(0.f, 0.175f, 0.1f));
	lightBody.GetMesh()->Transform = glm::scale(lightBody.GetMesh()->Transform, glm::vec3(0.5f, 0.1f, 1.f));
}
