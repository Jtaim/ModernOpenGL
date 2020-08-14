#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// globals
constexpr std::basic_string_view<char> APP_TITLE{"LearnOpenGL"};
constexpr int windowWidth{800};
constexpr int windowHeight{600};
constexpr bool fullscreen{false};

// forward function declarations
bool InitWindow(GLFWwindow*& window);
void processInput(GLFWwindow* window);
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);

const char* vertexShaderSrc{
R"(#version 330 core
layout(location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})"};

const char* fragmentShaderSrc{
R"(#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})"};

int main()
{
    GLFWwindow* window{};
    if(!InitWindow(window)){
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // vertex shader compilation
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        int errorLogLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLogLength);
        std::string errorLog(errorLogLength, ' ');
        glGetShaderInfoLog(vertexShader, errorLogLength, &errorLogLength, &errorLog.front());
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errorLog << std::endl;
    }

    // fragment shader compilation
    unsigned int fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        int errorLogLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &errorLogLength);
        std::string errorLog(errorLogLength, ' ');
        glGetShaderInfoLog(fragmentShader, errorLogLength, &errorLogLength, &errorLog.front());
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << errorLog << std::endl;
    }

    // shader linking
    unsigned int shaderProgram{glCreateProgram()};
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        int errorLogLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &errorLogLength);
        std::string errorLog(errorLogLength, ' ');
        glGetProgramInfoLog(shaderProgram, errorLogLength, &errorLogLength, &errorLog.front());
        std::cerr << "ERROR::SHADER::LINK_FAILED\n" << errorLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::array vertices{
        // first triangle
        -1.0f, -0.5f, 0.0f, // left
         0.0f, -0.5f, 0.0f, // right
        -0.5f,  0.5f, 0.0f,  // top
        // second triangle
         0.0f, -0.5f, 0.0f, // left
         1.0f, -0.5f, 0.0f, // right
         0.5f,  0.5f, 0.0f  // top
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

bool InitWindow(GLFWwindow*& window)
{
    if(!glfwInit()){
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if(fullscreen){
        GLFWmonitor* monitor{glfwGetPrimaryMonitor()};
        const GLFWvidmode* vmode{glfwGetVideoMode(monitor)};
        if(vmode != nullptr){
            window = glfwCreateWindow(vmode->width, vmode->height, APP_TITLE.data(), monitor, nullptr);
        }
    }
    else{
        window = glfwCreateWindow(windowWidth, windowHeight, APP_TITLE.data(), nullptr, nullptr);
    }

    if(window == nullptr){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "GLAD initialization failed" << std::endl;
        return false;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    return true;
}

void processInput(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch(key){
        case GLFW_KEY_ESCAPE:
            {
                if(action == GLFW_PRESS){
                    glfwSetWindowShouldClose(window, true);
                }
            }
            break;
        case GLFW_KEY_L:
            if(action == GLFW_PRESS || action == GLFW_REPEAT){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case GLFW_KEY_P:
            if(action == GLFW_PRESS || action == GLFW_REPEAT){
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                glPointSize(5.0f);
            }
            else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glPointSize(1.0f);
            }
    }
}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
