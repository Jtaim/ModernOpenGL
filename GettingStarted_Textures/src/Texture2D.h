#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>

#include <glad/glad.h>
#include <SOIL2/stb_image.h>

class Texture2D
{
public:
    Texture2D(const std::basic_string_view<char> filename);
    Texture2D(const Texture2D& other) = delete;
    Texture2D& operator=(const Texture2D& other) = delete;
    virtual ~Texture2D();

    void Bind(unsigned int unit = 0);

private:
    unsigned int mTexture;
};

#endif // !TEXTURE2D_H
