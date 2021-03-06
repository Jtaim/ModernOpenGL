#include <array>

#include "display.h"
#include "shader.h"
#include "texture2D.h"

void KeyCallback(Display::value_type* window, int key, int scancode, int action, int mods);

int main()
{
    Display window{800, 600, "LearnOpenGL"};
    window.SetKeyCallback(KeyCallback);

    Shader shader{"./shaders/texture.vert", "./shaders/texture_combined.frag"};

    std::array vertices{
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
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
    while(!window.IsClosed()){
        window.Clear(0.2f, 0.3f, 0.3f, 1.0f);

        texture1.Bind(0);
        texture2.Bind(1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
