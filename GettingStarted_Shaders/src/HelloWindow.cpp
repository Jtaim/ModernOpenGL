#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// globals
constexpr std::basic_string_view<GLchar> APP_TITLE{"LearnOpenGL"};
constexpr int windowWidth{800};
constexpr int windowHeight{600};
bool fullscreen{false};

// forward function declarations
bool InitWindow(GLFWwindow*& window);
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
    GLFWwindow* window{};
    if(!InitWindow(window)){
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // main loop
    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
    return true;
}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch(key){
        case GLFW_KEY_ESCAPE:
            {
                if(action == GLFW_PRESS){
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
            }
            break;
    }
}
