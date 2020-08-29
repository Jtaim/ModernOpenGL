#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>

#include <glad/glad.h>
#include <SOIL2/stb_image.h>

/*
load and bind 2D texture images
const std::basic_string_view<char> filename
bool flipImage = false, tell stb_image.h to flip loaded texture's on the y-axis.
GLenum wrapSStyle = GLREPEAT, is filter for GL_TEXTURE_WRAP_S
GLenum wrapTStyle = GLREPEAT, is filter for GL_TEXTURE_WRAP_T
GLenum minFilterStyle = GL_NEAREST_MIPMAP_LINEAR, is filter for GL_TEXTURE_MIN_FILTER
GLenum magFilterStyle = GL_LINEAR, is filter for GL_TEXTURE_MAG_FILTER
*/
class Texture2D
{
public:
    explicit Texture2D(const std::basic_string_view<char> filename,
                       bool flipImage = false,
                       GLenum wrapSStyle = GL_REPEAT,
                       GLenum wrapTStyle = GL_REPEAT,
                       GLenum minFilterStyle = GL_NEAREST_MIPMAP_LINEAR,
                       GLenum magFilterStyle = GL_LINEAR);

    virtual ~Texture2D();

    void Bind(unsigned int unit);

    Texture2D() = delete;
    Texture2D(const Texture2D& other) = delete;
    Texture2D(Texture2D&& other) = delete;
    Texture2D& operator=(const Texture2D& other) = delete;
    Texture2D& operator=(Texture2D&& other) = delete;

private:
    unsigned int mTexture;
};

#endif // !TEXTURE2D_H
