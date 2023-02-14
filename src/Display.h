#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Display
{
private:
    GLFWwindow *m_Window;
public:
    ~Display();

    bool Init() const;

    bool CreateWindow(const unsigned int width, const unsigned int height, const char *title, const bool resizable);

    void SetContext();

    bool WindowShouldClose();

    void SwapBuffers();

    void PollEvents();
};
