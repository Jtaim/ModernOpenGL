#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "Texture2D.h"

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

int main()
{
    GLFWwindow* window{};
    if(!InitWindow(window)){
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    ShaderProgram shader{"./shaders/texture.vert", "./shaders/texture_combined_ex1.frag"};

    std::array vertices{
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    std::array indices = {
        0u, 1u, 3u, // first triangle
        1u, 2u, 3u  // second triangle
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(3 * sizeof(vertices.front())));
    glEnableVertexAttribArray(1);
    // texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(6 * sizeof(vertices.front())));
    glEnableVertexAttribArray(2);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    Texture2D texture1{"./textures/container.jpg"};
    Texture2D texture2{"./textures/awesomeface.png"};
    
    shader.Bind(); // don't forget to activate the shader before setting uniforms!
    shader.SetUniform("texture1", 0);
    shader.SetUniform("texture2", 1);

    // render loop
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        texture1.Bind(0);
        texture2.Bind(1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
