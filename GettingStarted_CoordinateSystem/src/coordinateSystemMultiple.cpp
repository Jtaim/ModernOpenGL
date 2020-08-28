#include <array>
#include <iostream>

#include "display.h"
#include "shader.h"
#include "texture2D.h"

void KeyCallback(Display::value_type* window, int key, int scancode, int action, int mods);
void WindowSizeCallback(GLFWwindow* window, int width, int height);

// settings
constexpr unsigned int SCR_WIDTH{800};
constexpr unsigned int SCR_HEIGHT{600};

int main()
{
    Display window{SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL"};
    window.SetKeyCallback(KeyCallback);
    window.SetWindowSizeCallback(WindowSizeCallback);

    Shader shader{"./shaders/coordinate.vert", "./shaders/coordinate.frag"};

    std::array vertices{
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(3 * sizeof(vertices.front())));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    Texture2D texture1{"./textures/container.jpg"};
    Texture2D texture2{"./textures/awesomeface.png"};

    shader.Bind(); // don't forget to activate the shader before setting uniforms!
    shader.SetUniform("texture1", 0);
    shader.SetUniform("texture2", 1);

    texture1.Bind(0);
    texture2.Bind(1);

    std::array cubePositions{
        glm::vec3{0.0f, 0.0f, 0.0f},
        glm::vec3{2.0f, 5.0f, -15.0f},
        glm::vec3{-1.5f, -2.2f, -2.5f},
        glm::vec3{-3.8f, -2.0f, -12.3f},
        glm::vec3{2.4f, -0.4f, -3.5f},
        glm::vec3{-1.7f, 3.0f, -7.5f},
        glm::vec3{1.3f, -2.0f, -2.5f},
        glm::vec3{1.5f, 2.0f, -2.5f},
        glm::vec3{1.5f, 0.2f, -1.5f},
        glm::vec3{-1.3f, 1.0f, -1.5f}
    };

    glm::mat4 view{1.0f};
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    shader.SetUniformMatrix("view", view);

    glm::mat4 projection{1.0f};
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.SetUniformMatrix("projection", projection);

    // render loop
    while(!window.IsClosed()){
        window.Clear(0.2f, 0.3f, 0.3f, 1.0f);

        glBindVertexArray(VAO);
        for(size_t i{}; i < cubePositions.size(); ++i){
            glm::mat4 model{1.0f};
            model = glm::translate(model, cubePositions[i]);
            auto angle = glm::radians(20.0f * (float)i);
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            shader.SetUniformMatrix("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        // check and call events and swap buffers
        window.Update();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}

void KeyCallback(Display::value_type* window, int key, int scancode, int action, int mods)
{
    auto display = Display::GetWindowUserPointer(window);
    switch(key){
        case GLFW_KEY_ESCAPE:
        {
            if(action == GLFW_PRESS){
                display->SetClose();
            }
        }
        break;

        case GLFW_KEY_L:
        {
            if(action == GLFW_PRESS || action == GLFW_REPEAT){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        break;

        case GLFW_KEY_P:
        {
            if(action == GLFW_PRESS || action == GLFW_REPEAT){
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                glPointSize(2.0f);
            }
            else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glPointSize(1.0f);
            }
        }
        break;
    }
}

void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    //TODO later update any perspective matrices used here
}
