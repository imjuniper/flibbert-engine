#include "Display.h"

#include <iostream>
#include "Renderer.h"

Display::~Display()
{
    glfwTerminate();
}

bool Display::Init() const
{
    return glfwInit();
}

bool Display::CreateWindow(const unsigned int width, const unsigned int height, const char *title, const bool resizable)
{
    /* Set OpenGL version to 4.1 on macOS */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);

    /* Create a windowed mode window and its src context */
    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        return false;
    }

    return true;
}

void Display::SetContext()
{
    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Error" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    GLCallV(glEnable(GL_BLEND));
    GLCallV(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

bool Display::WindowShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Display::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

void Display::PollEvents()
{
    glfwPollEvents();
}

