#include "display.h"
#include <iostream>
#include <sstream>

Display::Display(int width, int height, const std::basic_string_view<char> title, bool fullscreen)
    : m_window{nullptr}, m_isClosed{}, m_width{width}, m_height{height}, m_windowName{title}
{
    glfwSetErrorCallback(DisplayErrorCallback);
    if(glfwInit() == GLFW_FALSE){
        std::cerr << "Display failed to initialize" << std::endl;
    }

    // set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // The monitor to use for full screen mode, or NULL for windowed mode.
    GLFWmonitor* monitor{};
    if(fullscreen){
        monitor = glfwGetPrimaryMonitor();
        const auto* vmode{glfwGetVideoMode(monitor)};
        m_window = glfwCreateWindow(vmode->width, vmode->height, m_windowName.c_str(), monitor, nullptr);
    }

    m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), monitor, nullptr);
    if(!m_window){
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_window);

    // glew: load all OpenGL function pointers
#ifdef __glew_h__
// glew: load all OpenGL function pointers
    glewExperimental = GL_TRUE; //insure gets modern OpenGL functions?
    if(glewInit() != GLEW_OK){
        std::cerr << "OpenGL failed to initialize" << std::endl;
        glfwTerminate();
    }
#else
#ifdef __glad_h_
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "OpenGL failed to initialize" << std::endl;
        glfwTerminate();
    }
#endif // __glad_h_
#endif // __glew_h__

    glfwSetWindowUserPointer(m_window, this);	// to use callback functions from classes
    glfwSetWindowCloseCallback(m_window, DisplayWindowCloseCallback);  // keep this internal

    glfwSetKeyCallback(m_window, m_keyCallback);
    glfwSetWindowSizeCallback(m_window, DisplayWindowSizeCallback);
    glfwSetFramebufferSizeCallback(m_window, DisplayFramebufferResizeCallback);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if(flags & GL_CONTEXT_FLAG_DEBUG_BIT){
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr); // TODO find OpenGL version this function only works with version >= 4.3
        // TODO all debug is open can make more sophisticated
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

Display::~Display()
{
    glfwTerminate();
}

void Display::Clear(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Display::Update()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool Display::IsClosed() const
{
    return m_isClosed;
}

int Display::GetHeight() const
{
    return m_height;
}

int Display::GetWidth() const
{
    return m_width;
}

void Display::SetClose()
{
    m_isClosed = true;
    glfwSetWindowShouldClose(m_window, true);
}

Display* Display::GetWindowUserPointer(GLFWwindow* window)
{
    return reinterpret_cast<Display*>(glfwGetWindowUserPointer(window));
}

void Display::SetKeyCallback(KeyCallback keyCallback)
{
    m_keyCallback = keyCallback;
    glfwSetKeyCallback(m_window, m_keyCallback);
}

void Display::DisplayErrorCallback(int error, const char* description)
{
    std::cerr << "ERROR: code: " << error << " msg: " << description << std::endl;
}

void Display::DisplayWindowCloseCallback(GLFWwindow* window)
{
    auto display = reinterpret_cast<Display*>(glfwGetWindowUserPointer(window));
    display->m_isClosed = true;
    glfwSetWindowShouldClose(window, true);
}

void Display::DisplayWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    if(window){
        auto display = static_cast<Display*>(glfwGetWindowUserPointer(window));
        display->m_width = width;
        display->m_height = height;
    }
}

void Display::DisplayFramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    if(window){
        auto display = reinterpret_cast<Display*>(glfwGetWindowUserPointer(window));
        display->m_width = width;
        display->m_height = height;
        glViewport(0, 0, width, height);
        //TODO later update any perspective matrices used here
    }
}

void Display::glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                           [[maybe_unused]] GLsizei length, const GLchar* message,
                           [[maybe_unused]] const void* userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;
    std::ostringstream os;
    os << "---------------\n";
    os << "Debug message (" << id << "): " << message << '\n';

    switch(source){
        case GL_DEBUG_SOURCE_API:
            os << "Source: API\n";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            os << "Source: Window System\n";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            os << "Source: Shader Compiler\n";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            os << "Source: Third Party\n";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            os << "Source: Application\n";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            os << "Source: Other\n";
            break;
    }

    switch(type){
        case GL_DEBUG_TYPE_ERROR:
            os << "Type: Error\n";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            os << "Type: Deprecated Behavior\n";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            os << "Type: Undefined Behavior\n";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            os << "Type: Portability\n";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            os << "Type: Performance\n";
            break;
        case GL_DEBUG_TYPE_MARKER:
            os << "Type: Marker\n";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            os << "Type: Push Group\n";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            os << "Type: Pop Group\n";
            break;
        case GL_DEBUG_TYPE_OTHER:
            os << "Type: Other\n";
            break;
    }

    switch(severity){
        case GL_DEBUG_SEVERITY_HIGH:
            os << "Severity: high\n";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            os << "Severity: medium\n";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            os << "Severity: low\n";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            os << "Severity: notification\n";
            break;
    }
    std::cout << os.str();
}
