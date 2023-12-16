#include <game_objects/tableTop.h>
#include <core/shapes.h>
#include <rendering/shader.h>
#include <rendering/types.h>
#include <glm/gtc/matrix_transform.hpp>

TableTop::TableTop()
{
	createShaders();
	createMesh();

}

void TableTop::Init()
{
}

void TableTop::Update(float deltaTime) {

}

void TableTop::Draw(const SceneParameters& sceneParams) {
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

void TableTop::ProcessLighting(SceneParameters& sceneParam) {
	//Not a light, Do do nothing
	return;
}

void TableTop::createShaders() {
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";
	_basicUnlitShader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");

	//Load cube mesh testures
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";
	_textures.emplace_back(texturePath / "wood2.jpg");
	_textures.emplace_back(texturePath / "");
}

void TableTop::createMesh() {
	auto plane = std::make_shared<Mesh>(Shapes::planeVertices, Shapes::planeElements, glm::vec3(1.f, 1.f, 1.f));
	auto& tableTop = _models.emplace_back(plane, _basicUnlitShader);
	tableTop.GetMesh()->Transform = glm::translate(tableTop.GetMesh()->Transform, glm::vec3(0.f, -0.35f, 1.f));
	tableTop.GetMesh()->Transform = glm::scale(tableTop.GetMesh()->Transform, glm::vec3(5.f, 1.f, 3.5f));
}
