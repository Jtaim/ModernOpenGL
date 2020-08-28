#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"


const std::map<const std::basic_string<char>, const GLenum> shaderTypes{
            {".vert", GL_VERTEX_SHADER},
            {".tesc", GL_TESS_CONTROL_SHADER},
            {".tese", GL_TESS_EVALUATION_SHADER},
            {".geom", GL_GEOMETRY_SHADER},
            {".frag", GL_FRAGMENT_SHADER}
};

class Shader
{
public:
    explicit Shader(const std::initializer_list<std::basic_string_view<char>> shaderFiles);
    ~Shader();

    void Bind() const;
    void Update();

    operator unsigned int() const;

    unsigned int GetUniformLocation(const std::basic_string_view<char> name) const;

    template<typename uniform>
    void SetUniform(const std::basic_string_view<char> name, const uniform& v) const;

    template<typename uniform>
    void SetUniformMatrix(const std::basic_string_view<char> name, const uniform& v) const;

    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(const Shader&&) = delete;

private:

    static unsigned int GetShader(const std::basic_string_view<char> shaderFile);
    static std::basic_string<char> LoadShader(const std::basic_string_view<char> fileName);
    static unsigned int CreateShader(const std::basic_string_view<char> shaderSrc, const GLenum type);
    static void CheckShaderError(unsigned int shader, bool isProgram, const std::basic_string_view<char> errorMessage);

    unsigned int mHandle;
    std::map<std::basic_string<char>, std::pair<GLenum, unsigned int>> mUniforms;
};

#endif  // SHADER_H

template<typename uniform>
inline void Shader::SetUniform(const std::basic_string_view<char> name, const uniform& v) const
{
    static_assert(std::is_same<uniform, bool>::value || std::is_same<uniform, glm::bvec1>::value ||
                  std::is_same<uniform, glm::bvec2>::value || std::is_same<uniform, glm::bvec3>::value ||
                  std::is_same<uniform, glm::bvec4>::value ||
                  std::is_same<uniform, float>::value || std::is_same<uniform, glm::vec1>::value ||
                  std::is_same<uniform, glm::vec2>::value || std::is_same<uniform, glm::vec3>::value ||
                  std::is_same<uniform, glm::vec4>::value ||
                  std::is_same<uniform, int>::value || std::is_same<uniform, glm::ivec1>::value ||
                  std::is_same<uniform, glm::ivec2>::value || std::is_same<uniform, glm::ivec3>::value ||
                  std::is_same<uniform, glm::ivec4>::value ||
                  std::is_same<uniform, unsigned int>::value || std::is_same<uniform, glm::uvec1>::value ||
                  std::is_same<uniform, glm::uvec2>::value || std::is_same<uniform, glm::uvec3>::value ||
                  std::is_same<uniform, glm::uvec4>::value,
                  "Type Invalid needs type of bool, float, int, unsigned int or corresponding glm::vec type");

    auto location{GetUniformLocation(name)};
    if(GetUniformLocation(name) == std::numeric_limits<unsigned int>::max()){
        std::cerr << "Error: Could not find location for " << name << std::endl;
        return;
    }
    // bool
    if constexpr(std::is_same<uniform, bool>::value){
        SetUniform(location, static_cast<int>(v));
    }
    else if constexpr(std::is_same<uniform, glm::bvec1>::value){
        SetUniform(location, static_cast<int>(v.x));
    }
    else if constexpr(std::is_same<uniform, glm::bvec2>::value){
        SetUniform(location, glm::ivec2{static_cast<int>(v.x), static_cast<int>(v.y)});
    }
    else if constexpr(std::is_same<uniform, glm::bvec3>::value){
        SetUniform(location, glm::ivec3{static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)});
    }
    else if constexpr(std::is_same<uniform, glm::bvec4>::value){
        SetUniform(location, glm::ivec3{static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), static_cast<int>(v.w)});
    }
    // float
    else if constexpr(std::is_same<uniform, float>::value){
        glUniform1f(location, v);
    }
    else if constexpr(std::is_same<uniform, glm::vec1>::value){
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
    // int
    else if constexpr(std::is_same<uniform, int>::value){
        glUniform1i(location, v);
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
    // unsigned int
    else if constexpr(std::is_same<uniform, unsigned int>::value){
        glUniform1ui(location, v);
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

template<typename uniform>
inline void Shader::SetUniformMatrix(const std::basic_string_view<char> name, const uniform& m) const
{
    static_assert(std::is_same<uniform, glm::mat2>::value || std::is_same<uniform, glm::mat3>::value ||
                  std::is_same<uniform, glm::mat4>::value ||
                  std::is_same<uniform, glm::mat2x3>::value || std::is_same<uniform, glm::mat2x4>::value ||
                  std::is_same<uniform, glm::mat3x2>::value || std::is_same<uniform, glm::mat3x4>::value ||
                  std::is_same<uniform, glm::mat4x2>::value || std::is_same<uniform, glm::mat4x3>::value,
                  "Type Invalid needs matrix type");

    auto location{GetUniformLocation(name)};
    if(GetUniformLocation(name) == std::numeric_limits<unsigned int>::max()){
        std::cerr << "Error: Could not find location for " << name << std::endl;
        return;
    }
    if constexpr(std::is_same<uniform, glm::mat2>::value){
        glUniformMatrix2fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat3>::value){
        glUniformMatrix3fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat4>::value){
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat2x3>::value){
        glUniformMatrix2x3fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat2x4>::value){
        glUniformMatrix2x4fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat3x2>::value){
        glUniformMatrix3x2fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat3x4>::value){
        glUniformMatrix3x4fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat4x2>::value){
        glUniformMatrix4x2fv(location, 1, false, glm::value_ptr(m));
    }
    else if constexpr(std::is_same<uniform, glm::mat4x3>::value){
        glUniformMatrix4x3fv(location, 1, false, glm::value_ptr(m));
    }
}
