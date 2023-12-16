#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;

//uniform sampler2D tex0;
//uniform sampler2D tex1;

void main() {
    FragColor = vertexColor;
}