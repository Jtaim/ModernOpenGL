#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

enum class ShaderType
{
    VERTEX,         // required shader
    TESSELLATION,   // optional shader
    GEOMETRY,       // optional shader
    FRAGMENT,       // required shader
    PROGRAM         // link shaders
};

class ShaderProgram
{
public:
    ShaderProgram(const std::basic_string_view<char> vsFilename, const std::basic_string_view<char> fsFilename);
    ~ShaderProgram();

    bool loadShaders(const std::basic_string_view<char> vsFilename, const std::basic_string_view<char> fsFilename);
    void bind() const;
    void Unbind() const;
    operator unsigned int()
    {
        return mHandle;
    }

    template<typename uniform>
    void setUniform(const std::basic_string_view<char> name, const uniform& v) const;

    ShaderProgram() = delete;
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&&) = delete;

private:

    std::basic_string<char> fileToString(const std::basic_string<char>& filename);
    void checkCompileErrors(unsigned int shader, ShaderType type) const;
    bool getUniformLocation(const std::basic_string_view<char> name, unsigned int& location) const;

    unsigned int mHandle;
    std::map<std::basic_string<char>, std::pair<GLenum, unsigned int>> mUniforms;
};

#endif  // SHADER_PROGRAM_H

template<typename uniform>
inline void ShaderProgram::setUniform(const std::basic_string_view<char> name, const uniform& v) const
{
    static_assert(std::is_same<uniform, glm::vec1>::value || std::is_same<uniform, glm::vec2>::value ||
                  std::is_same<uniform, glm::vec3>::value || std::is_same<uniform, glm::vec4>::value ||
                  std::is_same<uniform, glm::ivec1>::value || std::is_same<uniform, glm::ivec2>::value ||
                  std::is_same<uniform, glm::ivec3>::value || std::is_same<uniform, glm::ivec4>::value ||
                  std::is_same<uniform, glm::uvec1>::value || std::is_same<uniform, glm::uvec2>::value ||
                  std::is_same<uniform, glm::uvec3>::value || std::is_same<uniform, glm::uvec4>::value,
                  "Type Invalid needs type of glm::vec, glm::ivec, or glm::uvec");

    unsigned int location;
    if(!getUniformLocation(name, location)){
        std::cerr << "Error: Could not find location for " << name << std::endl;
        return;
    }

    if constexpr(std::is_same<uniform, glm::vec1>::value){
        glUniform1f(location, v.x);
    }
    else if constexpr(std::is_same<uniform, glm::vec2>::value){
        glUniform2f(location, v.x, v.y);
    }
    else if constexpr(std::is_same<uniform, glm::vec3>::value){
        glUniform3f(location, v.x, v.y, v.z);
    }
    else if constexpr(std::is_same<uniform, glm::vec4>::value){
        glUniform4f(location, v.x, v.y, v.z, v.w);
    }
    else if constexpr(std::is_same<uniform, glm::ivec1>::value){
        glUniform1i(location, v.x);
    }
    else if constexpr(std::is_same<uniform, glm::ivec2>::value){
        glUniform2i(location, v.x, v.y);
    }
    else if constexpr(std::is_same<uniform, glm::ivec3>::value){
        glUniform3i(location, v.x, v.y, v.z);
    }
    else if constexpr(std::is_same<uniform, glm::ivec4>::value){
        glUniform4i(location, v.x, v.y, v.z, v.w);
    }
    else if constexpr(std::is_same<uniform, glm::uvec1>::value){
        glUniform1ui(location, v.x);
    }
    else if constexpr(std::is_same<uniform, glm::uvec2>::value){
        glUniform2ui(location, v.x, v.y);
    }
    else if constexpr(std::is_same<uniform, glm::uvec3>::value){
        glUniform3ui(location, v.x, v.y, v.z);
    }
    else if constexpr(std::is_same<uniform, glm::uvec4>::value){
        glUniform4ui(location, v.x, v.y, v.z, v.w);
    }
}
