#include "shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(const std::initializer_list<std::basic_string_view<char>> shaderFiles)
    : mHandle{}, mUniforms{}
{
    mHandle = glCreateProgram();

    auto shaders{std::make_unique<unsigned int[]>(shaderFiles.size())};

    for(size_t i{}; i < shaderFiles.size(); ++i){
        shaders[i] = GetShader(*(shaderFiles.begin() + i));
        glAttachShader(mHandle, shaders[i]);
    }

    glLinkProgram(mHandle);
    CheckShaderError(mHandle, true, "Error linking shader program");

#if defined DEBUG || defined _DEBUG
    glValidateProgram(mHandle);
    CheckShaderError(mHandle, true, "Error Invalid shader program");
#endif // DEBUG || defined _DEBUG

    for(size_t i{}; i < shaderFiles.size(); ++i){
        glDeleteShader(shaders[i]);
    }

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

Shader::~Shader()
{
    glDeleteProgram(mHandle);
}

void Shader::Bind() const
{
    glUseProgram(mHandle);
}

void Shader::Update()
{
    // update transforms?
}

unsigned int Shader::GetShader(const std::basic_string_view<char> shaderFile)
{
    // check for valid file extension
    auto offset{shaderFile.find_last_of(".")};
    if(offset == std::string::npos){
        std::cerr << "Error file has no file extension " << shaderFile << "\n";
        return 0;
    }
    auto type{shaderTypes.find(shaderFile.substr(offset).data())};
    if(type == shaderTypes.end()){
        std::cerr << "Error invalid file extension " << shaderFile << "\n";
        return 0;
    }
    auto src{LoadShader(shaderFile)};
    return CreateShader(src.c_str(), type->second);
}

std::basic_string<char> Shader::LoadShader(const std::basic_string_view<char> fileName)
{
    std::basic_stringstream<char> ss;
    try{
        std::ifstream file(fileName.data());
        file.exceptions(file.exceptions() | std::ios::badbit | std::ios::failbit);
        ss << file.rdbuf();
        file.close();
    }
    catch(std::ifstream::failure e){
        std::cerr << "Error loading file " << fileName << "\n"
            << e.what() << std::endl;
    }
    return ss.str();
}

unsigned int Shader::CreateShader(const std::basic_string_view<char> shaderSrc, const GLenum type)
{
    auto src{reinterpret_cast<const char*>(shaderSrc.data())};
    int length[]{static_cast<int>(shaderSrc.length())};

    auto id{glCreateShader(type)};
    glShaderSource(id, 1, &src, length);
    glCompileShader(id);
    // check for shader compile errors
    std::basic_string<char> errorMsg;
    switch(type){
        case GL_VERTEX_SHADER:
            errorMsg = "Error compiling vertex shader!";
            break;
        case GL_GEOMETRY_SHADER:
            errorMsg = "Error compiling geometry shader!";
            break;
        case GL_TESS_CONTROL_SHADER:
            errorMsg = "Error compiling tess_ctrl shader!";
            break;
        case GL_TESS_EVALUATION_SHADER:
            errorMsg = "Error compiling tess_eval shader!";
            break;
        case GL_FRAGMENT_SHADER:
            errorMsg = "Error compiling fragment shader!";
            break;
        default:
            errorMsg = "Error unknown shader!";
    }
    CheckShaderError(id, false, errorMsg);

    return id;
}

void Shader::CheckShaderError(unsigned int shader, bool isProgram, const std::basic_string_view<char> errorMessage)
{
    GLint success;

    if(isProgram){
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
    }
    else{
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    }

    if(success == GL_FALSE){
        int infoLogLength;
        std::unique_ptr<char[]> errorLog;
        if(isProgram){
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            errorLog = std::make_unique<char[]>(infoLogLength);
            glGetProgramInfoLog(shader, infoLogLength, &infoLogLength, errorLog.get());
        }
        else{
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            errorLog = std::make_unique<char[]>(infoLogLength);
            glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, errorLog.get());
        }
        std::cerr << errorMessage << ": '" << errorLog << "'" << std::endl;
    }
}

bool Shader::GetUniformLocation(const std::basic_string_view<char> name, unsigned int& location) const
{
    auto it{mUniforms.find(name.data())};
    if(it == mUniforms.end()){
        return false;
    }
    location = it->second.second;
    return true;
}
