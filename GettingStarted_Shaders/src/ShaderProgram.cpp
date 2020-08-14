#include "ShaderProgram.h"
#include <fstream>
#include <utility>
#include <sstream>

ShaderProgram::ShaderProgram(const std::basic_string_view<char> vsFilename, const std::basic_string_view<char> fsFilename)
    : mHandle{}, mUniforms{}
{
    loadShaders(vsFilename, fsFilename);
    if(mHandle){
        int numberOfUniforms;
        int maxNameSize;
        unsigned int location;
        glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
        glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
        for(decltype(numberOfUniforms)i{}; i < numberOfUniforms; ++i){
            GLenum type;
            int size;
            std::basic_string<char> name(maxNameSize, '\0');
            glGetActiveUniform(mHandle, i, name.size(), nullptr, &size, &type, &name[0]);
            location = glGetUniformLocation(mHandle, name.c_str());
            mUniforms.insert(std::make_pair(name.c_str(), std::make_pair(type, location)));
        }
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mHandle);
}

bool ShaderProgram::loadShaders(const std::basic_string_view<GLchar> vsFilename, const std::basic_string_view<GLchar> fsFilename)
{
    auto vsString{fileToString(vsFilename.data())};
    auto fsString{fileToString(fsFilename.data())};
    if(vsString.empty() || fsString.empty()){
        return false;
    }
    const char* vsSourcePtr{vsString.c_str()};
    const char* fsSourcePtr{fsString.c_str()};

    auto vs{glCreateShader(GL_VERTEX_SHADER)};
    auto fs{glCreateShader(GL_FRAGMENT_SHADER)};

    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);

    glCompileShader(vs);
    checkCompileErrors(vs, ShaderType::VERTEX);
    glCompileShader(fs);
    checkCompileErrors(fs, ShaderType::FRAGMENT);

    mHandle = glCreateProgram();
    glAttachShader(mHandle, vs);
    glAttachShader(mHandle, fs);
    glLinkProgram(mHandle);
    checkCompileErrors(mHandle, ShaderType::PROGRAM);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}

void ShaderProgram::bind() const
{
    if(mHandle > 0){
        glUseProgram(mHandle);
    }
}

void ShaderProgram::Unbind() const
{
    glDeleteProgram(mHandle);
}

std::basic_string<char> ShaderProgram::fileToString(const std::basic_string<char>& filename)
{
    std::basic_stringstream<char> ss;

    try{
        std::ifstream file{filename, std::ios::in};
        file.exceptions(file.exceptions() | std::ios::badbit);
        if(file.is_open()){
            ss << file.rdbuf();
            file.close();
        }
        else{
            std::cout << "Error could not find file " << filename << std::endl;
        }
    }
    catch(std::exception){
        std::cerr << "Critical Error reading shader filename!" << std::endl;
    }

    return ss.str();
}

void ShaderProgram::checkCompileErrors(unsigned int shader, ShaderType type) const
{
    int status;
    int infoLogLength;

    if(type == ShaderType::PROGRAM){
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if(status == GL_FALSE){
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::string errorLog(infoLogLength, ' ');
            glGetProgramInfoLog(mHandle, infoLogLength, &infoLogLength, &errorLog.front());
            std::cerr << "Error shader program linker failure.\n" << errorLog << std::endl;
        }
    }
    else{   // vertex or fragment compiling
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE){
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::string errorLog(infoLogLength, ' ');
            glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, &errorLog.front());
            if(type == ShaderType::VERTEX){
                std::cerr << "Error Vertex Shader failed to compile.\n" << errorLog << std::endl;
            }
            else if(type == ShaderType::FRAGMENT){
                std::cerr << "Error Fragment Shader failed to compile.\n" << errorLog << std::endl;
            }
            else{
                std::cerr << "Error Unknown Shader compiler failure.\n" << errorLog << std::endl;
            }
        }
    }
}

bool ShaderProgram::getUniformLocation(const std::basic_string_view<char> name, unsigned int& location) const
{
    auto it{mUniforms.find(name.data())};
    if(it == mUniforms.end()){
        return false;
    }
    location = it->second.second;
    return true;
}
