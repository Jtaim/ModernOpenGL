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
        // positions        // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left 
    };

    std::array indices = {
        0u, 1u, 3u, // first triangle
        1u, 2u, 3u  // second triangle
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(3 * sizeof(vertices.front())));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Texture2D texture1{"./textures/container.jpg"};
    Texture2D texture2{"./textures/awesomeface.png", true};

    shader.Bind(); // don't forget to activate the shader before setting uniforms!
    shader.SetUniform("texture1", 0);
    shader.SetUniform("texture2", 1);

    auto model{glm::rotate(glm::mat4{1.0f}, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f))};
    auto view{glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, -3.0f))};
    auto projection{glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)};

    texture1.Bind(0);
    texture2.Bind(1);

    // render loop
    while(!window.IsClosed()){
        window.Clear(0.2f, 0.3f, 0.3f, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        shader.SetUniformMatrix("model", model);
        shader.SetUniformMatrix("view", view);
        shader.SetUniformMatrix("projection", projection);

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
