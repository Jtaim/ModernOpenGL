#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>

#include <glad/glad.h>
#include <SOIL2/stb_image.h>

class Texture2D
{
public:
    explicit Texture2D(const std::basic_string_view<char> filename, GLenum wrapStyle = GL_REPEAT, GLenum filterStyle = GL_LINEAR);

    virtual ~Texture2D();

    void Bind(unsigned int unit = 0);

    Texture2D() = delete;
    Texture2D(const Texture2D& other) = delete;
    Texture2D(Texture2D&& other) = delete;
    Texture2D& operator=(const Texture2D& other) = delete;
    Texture2D& operator=(Texture2D&& other) = delete;

private:
    unsigned int mTexture;
};

#endif // !TEXTURE2D_H
