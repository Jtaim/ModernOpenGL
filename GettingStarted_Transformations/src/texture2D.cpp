#include "Texture2D.h"
#include <cassert>
#include <iostream>

Texture2D::Texture2D(const std::basic_string_view<char> filename, const GLenum wrapStyle, GLenum filterStyle)
    : mTexture{}
{
    assert(wrapStyle == GL_CLAMP_TO_EDGE || wrapStyle == GL_MIRRORED_REPEAT || wrapStyle == GL_REPEAT);
    assert(filterStyle == GL_NEAREST || filterStyle == GL_LINEAR);

    int width, height, components;
    unsigned char* imageData{stbi_load(filename.data(), &width, &height, &components, 4)};

    if(!imageData){
        std::cerr << "Texture loading failed: " << filename << std::endl;
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapStyle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapStyle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterStyle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterStyle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &mTexture);
}

void Texture2D::Bind(unsigned int unit)
{
    assert(unit >= 0 && unit <= 15);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}
