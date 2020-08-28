#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

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

    /*template<typename uniform>
    void SetUniform(const std::basic_string_view<char> name, const uniform& v) const;*/

    template<typename uniform, typename ...uniformn>
    void SetUniform(const std::basic_string_view<char> name, const uniform arg, const uniformn... args) const;

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

//template<typename uniform>
//inline void Shader::SetUniform(const std::basic_string_view<char> name, const uniform& v) const
//{
//    static_assert(std::is_same<uniform, glm::vec1>::value || std::is_same<uniform, glm::vec2>::value ||
//                  std::is_same<uniform, glm::vec3>::value || std::is_same<uniform, glm::vec4>::value ||
//                  std::is_same<uniform, glm::ivec1>::value || std::is_same<uniform, glm::ivec2>::value ||
//                  std::is_same<uniform, glm::ivec3>::value || std::is_same<uniform, glm::ivec4>::value ||
//                  std::is_same<uniform, glm::uvec1>::value || std::is_same<uniform, glm::uvec2>::value ||
//                  std::is_same<uniform, glm::uvec3>::value || std::is_same<uniform, glm::uvec4>::value,
//                  "Type Invalid needs type of glm::vec, glm::ivec, or glm::uvec");
//
//    auto location{GetUniformLocation(name)};
//    if(GetUniformLocation(name) == std::numeric_limits<unsigned int>::max()){
//        std::cerr << "Error: Could not find location for " << name << std::endl;
//        return;
//    }
//
//    if constexpr(std::is_same<uniform, glm::vec1>::value){
//        glUniform1f(location, v.x);
//    }
//    else if constexpr(std::is_same<uniform, glm::vec2>::value){
//        glUniform2f(location, v.x, v.y);
//    }
//    else if constexpr(std::is_same<uniform, glm::vec3>::value){
//        glUniform3f(location, v.x, v.y, v.z);
//    }
//    else if constexpr(std::is_same<uniform, glm::vec4>::value){
//        glUniform4f(location, v.x, v.y, v.z, v.w);
//    }
//    else if constexpr(std::is_same<uniform, glm::ivec1>::value){
//        glUniform1i(location, v.x);
//    }
//    else if constexpr(std::is_same<uniform, glm::ivec2>::value){
//        glUniform2i(location, v.x, v.y);
//    }
//    else if constexpr(std::is_same<uniform, glm::ivec3>::value){
//        glUniform3i(location, v.x, v.y, v.z);
//    }
//    else if constexpr(std::is_same<uniform, glm::ivec4>::value){
//        glUniform4i(location, v.x, v.y, v.z, v.w);
//    }
//    else if constexpr(std::is_same<uniform, glm::uvec1>::value){
//        glUniform1ui(location, v.x);
//    }
//    else if constexpr(std::is_same<uniform, glm::uvec2>::value){
//        glUniform2ui(location, v.x, v.y);
//    }
//    else if constexpr(std::is_same<uniform, glm::uvec3>::value){
//        glUniform3ui(location, v.x, v.y, v.z);
//    }
//    else if constexpr(std::is_same<uniform, glm::uvec4>::value){
//        glUniform4ui(location, v.x, v.y, v.z, v.w);
//    }
//}

template<typename uniform, typename ...uniformn>
inline void Shader::SetUniform(const std::basic_string_view<char> name, const uniform arg, const uniformn... args) const
{
    static_assert((1 + sizeof...(args) <= 4), "basic type uniforms can only have 1 to 4 arguments");
    static_assert((std::is_same<uniform, uniformn>::value && ...), "basic type uniform arguments must be of same type");
    static_assert(std::is_same<uniform, int>::value || std::is_same<uniform, unsigned int>::value ||
                  std::is_same<uniform, float>::value || std::is_same<uniform, bool>::value,
                  "basic type uniforms only use types of int, unsigned int, and float");

    auto location{GetUniformLocation(name)};
    if(GetUniformLocation(name) == std::numeric_limits<unsigned int>::max()){
        std::cerr << "Error: Could not find location for " << name << std::endl;
        return;
    }
    if constexpr(std::is_same<uniform, bool>::value && (1 + sizeof...(args) == 1)){
        SetUniform(name, static_cast<int>(arg));
    }
    else if constexpr(std::is_same<uniform, bool>::value && (1 + sizeof...(args) == 2)){
        SetUniform(name, static_cast<int>(arg), static_cast<int>(args[0]), static_cast<int>(arg)[1]);
    }
    else if constexpr(std::is_same<uniform, bool>::value && (1 + sizeof...(args) == 3)){
        SetUniform(name, static_cast<int>(arg), static_cast<int>(args[0]), static_cast<int>(arg)[1], static_cast<int>(arg)[2]);
    }
    else if constexpr(std::is_same<uniform, bool>::value && (1 + sizeof...(args) == 4)){
        SetUniform(name, static_cast<int>(arg), static_cast<int>(args[0]), static_cast<int>(arg)[1],
                   static_cast<int>(arg)[2], static_cast<int>(arg)[3]);
    }
    else if constexpr(std::is_same<uniform, int>::value && (1 + sizeof...(args) == 1)){
        glUniform1i(location, arg);
    }
    else if constexpr(std::is_same<uniform, int>::value && (1 + sizeof...(args) == 2)){
        glUniform2i(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, int>::value && (1 + sizeof...(args) == 3)){
        glUniform3i(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, int>::value && (1 + sizeof...(args) == 4)){
        glUniform4i(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, unsigned int>::value && (1 + sizeof...(args) == 1)){
        glUniform1ui(location, arg);
    }
    else if constexpr(std::is_same<uniform, unsigned int>::value && (1 + sizeof...(args) == 2)){
        glUniform2ui(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, unsigned int>::value && (1 + sizeof...(args) == 3)){
        glUniform3ui(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, unsigned int>::value && (1 + sizeof...(args) == 4)){
        glUniform4ui(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, float>::value && (1 + sizeof...(args) == 1)){
        glUniform1f(location, arg);
    }
    else if constexpr(std::is_same<uniform, float>::value && (1 + sizeof...(args) == 2)){
        glUniform2f(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, float>::value && (1 + sizeof...(args) == 3)){
        glUniform3f(location, arg, args...);
    }
    else if constexpr(std::is_same<uniform, float>::value && (1 + sizeof...(args) == 4)){
        glUniform4f(location, arg, args...);
    }
}
