#include <game_objects/computer.h>
#include <core/shapes.h>
#include <rendering/shader.h>
#include <rendering/types.h>
#include <glm/gtc/matrix_transform.hpp>

Computer::Computer()
{
	createShaders();
	createMesh();

}

void Computer::Init()
{
}

void Computer::Update(float deltaTime) {

}

void Computer::Draw(const SceneParameters& sceneParams) {
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

void Computer::ProcessLighting(SceneParameters& sceneParam) {
	//Not a light, Do do nothing
	return;
}

void Computer::createShaders() {
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";
	_shader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");

	//Load cube mesh testures
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";
	_textures.emplace_back(texturePath / "alumium2.jpg");
	_textures.emplace_back(texturePath / "apple1.png");
}

void Computer::createMesh() {
	auto cube = std::make_shared<Mesh>(Shapes::cubeVertices, Shapes::cubeElements, glm::vec3(1.f, 1.f, 1.f));
	auto& computerTop = _models.emplace_back(cube, _shader);
	computerTop.GetMesh()->Transform = glm::translate(computerTop.GetMesh()->Transform, glm::vec3(0.f, -0.835f, 1.f));
	computerTop.GetMesh()->Transform = glm::scale(computerTop.GetMesh()->Transform, glm::vec3(2.5f, 0.025f, 1.8f));
}
