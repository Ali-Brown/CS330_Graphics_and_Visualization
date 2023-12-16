#include <texture.h>
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::filesystem::path& path)
{
    stbi_set_flip_vertically_on_load(true);

    auto texturePath = path.string();

    int width, height, numChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numChannels, STBI_rgb_alpha); //STBI_rgb_alpha = 4

    glGenTextures(1, &_textureHandle);
    glBindTexture(GL_TEXTURE_2D, _textureHandle);

    if (data) {
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height); // GLFW version agnostic using 4.2
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture at path: " << texturePath << std::endl;
    }

    stbi_image_free(data);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, _textureHandle);

    //UV Wrap Parameter Objects pair sample: currently not used no tiled uv supplied
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
