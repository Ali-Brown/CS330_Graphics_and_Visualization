#include <mesh.h>
#include <iostream> 
#include <core/shapes.h>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<uint32_t> &elements)
{
    init(vertices, elements);
}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<uint32_t>& elements, const glm::vec3 &color) {
    for (auto& vertex : vertices) {
        vertex.Color = color;
    }

    init(vertices, elements);
}

void Mesh::Draw() const {
    //ind vertex array 
    glBindVertexArray(_vertexArrayObject);

    // Draw mesh elements
    glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, nullptr);
}

void Mesh::init(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements) {
    // Auto-generate Normals: Could be added manually in shapes.h
    // but better auto generated because of complex shapes
    
    for (auto i = 0; i < elements.size(); i += 3) {
        auto p1Index = elements[i];
        auto p2Index = elements[i + 1];
        auto p3Index = elements[i + 2];

        Shapes::UpdateNormals(vertices[p1Index], vertices[p2Index], vertices[p3Index]);
    }

    // Generate and Bind Buffers(vertex and elements) and Vertex Array Objects
    glGenVertexArrays(1, &_vertexArrayObject);
    glGenBuffers(1, &_vertexBufferObject);
    glGenBuffers(1, &_elementBufferObject);

    glBindVertexArray(_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(elements.size() * sizeof(uint32_t)), elements.data(), GL_STATIC_DRAW);


    //Define vertex attribute for each channel/attribute in Vertex struct
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uv));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    _elementCount = elements.size();
}
