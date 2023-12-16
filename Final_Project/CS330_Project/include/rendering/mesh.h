#pragma once

#include <vector>
#include <rendering/types.h>
#include <glad/glad.h>      // Glad library

class Mesh {
public:
	Mesh(std::vector<Vertex> &vertices, std::vector<uint32_t> &elements);
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, const glm::vec3& color);

	void Draw() const;

	glm::mat4 Transform { 1.f };

private:
	void init(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements);

private:

	uint32_t _elementCount{0};
	GLuint _vertexArrayObject{};
	GLuint _vertexBufferObject{};
	GLuint _shaderProgram{};
	GLuint _elementBufferObject{};
};