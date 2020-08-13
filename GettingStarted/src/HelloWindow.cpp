#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// globals
constexpr std::basic_string_view<GLchar> APP_TITLE{"LearnOpenGL"};
constexpr int windowWidth{800};
constexpr int windowHeight{600};
constexpr bool fullscreen{false};

// forward function declarations
bool InitWindow(GLFWwindow*& window);
void processInput(GLFWwindow* window);
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main()
{
    GLFWwindow* window{};
    if(!InitWindow(window)){
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // render loop
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);


        // check and call events and swap buffers
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

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
