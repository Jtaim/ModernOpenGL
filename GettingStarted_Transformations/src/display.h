#ifndef DISPLAY_H_08162020
#define DISPLAY_H_08162020

#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Display
{
private:
    using KeyCallback = void(*)(GLFWwindow*, int, int, int, int);
public:
    using value_type = GLFWwindow;

    explicit Display(int width, int height, const std::basic_string_view<char> title, bool fullscreen = false);

    ~Display();

    void Clear(float r, float g, float b, float a) const;
    void Update();
    bool IsClosed() const;

    int GetHeight() const;
    int GetWidth() const;

    void SetClose();
    static Display* GetWindowUserPointer(GLFWwindow* window);
    inline operator GLFWwindow* () { return m_window; }

    void SetKeyCallback(KeyCallback keyCallback);

    Display(const Display& other) = delete;
    Display& operator=(const Display& other) = delete;
    Display(const Display&& other) = delete;
    Display& operator=(Display&& other) = delete;

private:
    static void DisplayErrorCallback(int error, const char* description);
    static void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userParam);
    static void DisplayWindowCloseCallback(GLFWwindow* window);

    static void DisplayWindowSizeCallback(GLFWwindow* window, int width, int height);
    static void DisplayFramebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    bool m_isClosed;

    int m_width;
    int m_height;
    std::string m_windowName;

    KeyCallback m_keyCallback = nullptr;
};

#endif /* WINDOW_H_08162020 */
