#include "ShaderProgram.h"
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram(const std::basic_string_view<char> vsFilename, const std::basic_string_view<char> fsFilename)
    : mHandle{}, mUniforms{}
{
    LoadShaders(vsFilename, fsFilename);
    if(mHandle){
        int numberOfUniforms;
        int maxNameSize;
        unsigned int location;
        glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
        glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
        for(decltype(numberOfUniforms)i{}; i < numberOfUniforms; ++i){
            GLenum type;
            int size;
            auto name{std::make_unique<char[]>(maxNameSize)};
            glGetActiveUniform(mHandle, i, maxNameSize, nullptr, &size, &type, name.get());
            location = glGetUniformLocation(mHandle, name.get());
            mUniforms.insert(std::make_pair(name.get(), std::make_pair(type, location)));
        }
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mHandle);
}

bool ShaderProgram::LoadShaders(const std::basic_string_view<char> vsFilename, const std::basic_string_view<char> fsFilename)
{
    auto vsString{FileToString(vsFilename.data())};
    auto fsString{FileToString(fsFilename.data())};

    const char* vsSourcePtr{vsString.c_str()};
    const char* fsSourcePtr{fsString.c_str()};

    auto vs{glCreateShader(GL_VERTEX_SHADER)};
    auto fs{glCreateShader(GL_FRAGMENT_SHADER)};

    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);

    glCompileShader(vs);
    CheckCompileErrors(vs, ShaderType::VERTEX);
    glCompileShader(fs);
    CheckCompileErrors(fs, ShaderType::FRAGMENT);

    mHandle = glCreateProgram();
    glAttachShader(mHandle, vs);
    glAttachShader(mHandle, fs);
    glLinkProgram(mHandle);
    CheckCompileErrors(mHandle, ShaderType::PROGRAM);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}

void ShaderProgram::Bind() const
{
    if(mHandle > 0){
        glUseProgram(mHandle);
    }
}

void ShaderProgram::Unbind() const
{
    glDeleteProgram(mHandle);
}

std::basic_string<char> ShaderProgram::FileToString(const std::basic_string<char>& filename)
{
    std::basic_stringstream<char> ss;
    try{
        std::ifstream file(filename);
        file.exceptions(file.exceptions() | std::ios::badbit | std::ios::failbit);
        ss << file.rdbuf();
        file.close();
        if(ss.str().empty()){
            std::cerr << "Error " << filename << " was found but is empty " << std::endl;
        }
    }
    catch(std::ifstream::failure e){
        std::cerr << "Error could not find file " << filename << "\n"
            << e.what() << std::endl;
    }
    return ss.str();
}

void ShaderProgram::CheckCompileErrors(unsigned int shader, ShaderType type) const
{
    int status;
    int infoLogLength;

    if(type == ShaderType::PROGRAM){
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if(status == GL_FALSE){
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
            auto errorLog{std::make_unique<char[]>(infoLogLength)};
            glGetProgramInfoLog(mHandle, infoLogLength, &infoLogLength, errorLog.get());
            std::cerr << "Error shader program failed to link.\n" << errorLog << std::endl;
        }
    }
    else{   // vertex or fragment compiling
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE){
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            auto errorLog{std::make_unique<char[]>(infoLogLength)};
            glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, errorLog.get());
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

bool ShaderProgram::GetUniformLocation(const std::basic_string_view<char> name, unsigned int& location) const
{
    auto it{mUniforms.find(name.data())};
    if(it == mUniforms.end()){
        return false;
    }
    location = it->second.second;
    return true;
}
