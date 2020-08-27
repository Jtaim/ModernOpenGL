#ifndef DISPLAY_H_08162020
#define DISPLAY_H_08162020

#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Display
{
private:
    using KeyCallback = void(*)(GLFWwindow*, int key, int scancode, int action, int mods);
    using WindowSizeCallback = void (*)(GLFWwindow* window, int width, int height);

public:
    using value_type = GLFWwindow;

    explicit Display(int width, int height, const std::basic_string_view<char> title, bool fullscreen = false);

    ~Display();

    void Clear(float r, float g, float b, float a) const;
    void Update();
    bool IsClosed() const;

    void SetClose();
    static Display* GetWindowUserPointer(GLFWwindow* window);
    inline operator GLFWwindow* () { return m_window; }

    void SetKeyCallback(KeyCallback keyCallback);
    void SetWindowSizeCallback(WindowSizeCallback resizeCallback);

    Display(const Display& other) = delete;
    Display& operator=(const Display& other) = delete;
    Display(const Display&& other) = delete;
    Display& operator=(Display&& other) = delete;

private:
    static void DisplayErrorCallback(int error, const char* description);
    static void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userParam);
    static void DisplayWindowCloseCallback(GLFWwindow* window);

    GLFWwindow* m_window;
    bool m_isClosed;

    std::string m_windowName;

    KeyCallback m_keyCallback = nullptr;
    WindowSizeCallback m_resizeCallback = nullptr;
};

#endif /* WINDOW_H_08162020 */
