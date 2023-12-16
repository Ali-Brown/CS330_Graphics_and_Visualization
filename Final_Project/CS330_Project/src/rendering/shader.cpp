#include <shader.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) {
	load(vertexSource, fragmentSource);
}

Shader::Shader(const Path &vertexPath, const Path &fragmentPath) {

    try {
        //load shader sources from files from shaders file
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        //load shader
        load(vShaderStream.str(), fShaderStream.str());
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

	
}

void Shader::Bind() {
    //using the shader program
    glUseProgram(_shaderProgram);
}

void Shader::load(const std::string &vertexSource, const std::string &fragmentSource) {
    //Compile vertex shader

    const char* vShaderCode = vertexSource.c_str();
    const char* fShaderCode = fragmentSource.c_str();

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Compile fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMNET::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Shader Program to link vertex and fragment shaders
    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    //Delete the shaders after shader program compilation
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLint Shader::getUniformLocation(const std::string& uniformName) const {
    return glGetUniformLocation(_shaderProgram, uniformName.c_str());
}


void Shader::SetVec3(const std::string& uniformName, const glm::vec3& vec3) const {
    auto uniformLoc = getUniformLocation(uniformName);

    if (uniformLoc != -1) {
        glUniform3fv(uniformLoc, 1, glm::value_ptr(vec3));
    }
}

void Shader::SetMat4(const std::string& uniformName, const glm::mat4& mat4) {
    auto uniformLoc = getUniformLocation(uniformName);

    if (uniformLoc != -1) {
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mat4));
    }
    
}


void Shader::AddTexture(const std::shared_ptr<Texture>& texture)
{
    _textures.emplace_back(texture);
}


void Shader::SetInt(const std::string& uniformName, int value) {
    auto uniformLoc = getUniformLocation(uniformName);

    if (uniformLoc != -1) {
        glUniform1i(uniformLoc, value);
    }
}

void Shader::SetFloat(const std::string& uniformName, float value) const {
    auto uniformLoc = getUniformLocation(uniformName);

    if (uniformLoc != -1) {
        glUniform1f(uniformLoc, value);
    }
}
