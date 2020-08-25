#include <array>

#include "display.h"
#include "shader.h"
#include "texture2D.h"

void KeyCallback(Display::value_type* window, int key, int scancode, int action, int mods);

int main()
{
    Display display{800, 600, "LearnOpenGL"};
    display.SetKeyCallback(KeyCallback);

    Shader shader{"./shaders/texture_wall.vert", "./shaders/texture_wall.frag"};

    std::array vertices{
        // positions        // texture coords
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // left
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // right
         0.0f,  0.5f, 0.0f, 0.5f, 1.0f  // top
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(3 * sizeof(vertices.front())));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load and create a texture 
    Texture2D texture{"./textures/wall.jpg"};

    // render loop
    while(!display.IsClosed()){
        display.Clear(0.2f, 0.3f, 0.3f, 1.0f);

        shader.Bind();
        texture.Bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        display.Update();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
