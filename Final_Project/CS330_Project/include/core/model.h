#pragma once

#include <rendering/mesh.h>
#include <rendering/shader.h>

//The model class wraps a mesh and it's shader
class Model {
public:
	Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader);
	Shader* GetShader() { return _shader.get(); }
	Mesh* GetMesh() { return _mesh.get(); }

private:
	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Mesh> _mesh;
};