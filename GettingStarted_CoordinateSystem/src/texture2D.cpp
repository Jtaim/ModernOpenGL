#include "Texture2D.h"
#include <cassert>
#include <iostream>

Texture2D::Texture2D(const std::basic_string_view<char> filename,
                     bool flipImage,
                     GLenum wrapSStyle,
                     GLenum wrapTStyle,
                     GLenum minFilterStyle,
                     GLenum magFilterStyle)
    : mTexture{}
{
    assert(wrapSStyle == GL_CLAMP_TO_EDGE || wrapSStyle == GL_CLAMP_TO_BORDER || 
           wrapSStyle == GL_MIRRORED_REPEAT || wrapSStyle == GL_REPEAT ||
           wrapSStyle == GL_MIRROR_CLAMP_TO_EDGE);
    assert(wrapTStyle == GL_CLAMP_TO_EDGE || wrapTStyle == GL_CLAMP_TO_BORDER ||
           wrapTStyle == GL_MIRRORED_REPEAT || wrapTStyle == GL_REPEAT ||
           wrapTStyle == GL_MIRROR_CLAMP_TO_EDGE);
    assert(minFilterStyle == GL_NEAREST || minFilterStyle == GL_LINEAR ||
           minFilterStyle == GL_NEAREST_MIPMAP_NEAREST || minFilterStyle == GL_LINEAR_MIPMAP_NEAREST ||
           minFilterStyle == GL_NEAREST_MIPMAP_LINEAR || minFilterStyle == GL_LINEAR_MIPMAP_LINEAR);
    assert(magFilterStyle == GL_NEAREST || magFilterStyle == GL_LINEAR);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(flipImage); // tell stb_image.h to flip loaded texture's on the y-axis.

    int width, height, components;
    unsigned char* imageData{stbi_load(filename.data(), &width, &height, &components, 4)};

    if(!imageData){
        std::cerr << "Texture loading failed: " << filename << std::endl;
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapSStyle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTStyle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterStyle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterStyle);

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
