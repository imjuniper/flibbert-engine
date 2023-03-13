#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <iostream>

#include "Application.h"
#include "opengl/Renderer.h"

#include "demos/Demo.h"
#include "demos/DemoCamera3D.h"
#include "demos/DemoClearColor.h"
#include "demos/DemoTexture2D.h"

static Application* s_Instance = nullptr;

Application::Application() {
    s_Instance = this;
    Init();
}

Application::~Application() {
    Shutdown();
    s_Instance = nullptr;
}

Application& Application::Get()
{
    return *s_Instance;
}

void Application::Init() {
    if (!glfwInit())
        return;

    /* Set OpenGL version to 4.1 on macOS */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    /* Create a windowed mode window and its src context */
    m_WindowHandle = glfwCreateWindow(960, 540, "OpenGL Learning", nullptr, nullptr);

    if (!m_WindowHandle)
        return;

    glfwMakeContextCurrent(m_WindowHandle);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Error" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle, true);
    ImGui::StyleColorsDark();
}

void Application::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void Application::Run() {
    Renderer renderer;

    Demo::Demo* currentDemo = nullptr;
    Demo::DemoMenu* demoMenu = new Demo::DemoMenu(currentDemo);
    currentDemo = demoMenu;

    demoMenu->RegisterDemo<Demo::DemoClearColor>("Clear Color");
    demoMenu->RegisterDemo<Demo::DemoTexture2D>("2D Texture");
    demoMenu->RegisterDemo<Demo::DemoCamera3D>("3D Camera");

    while (!glfwWindowShouldClose(m_WindowHandle)) {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentDemo) {
            currentDemo->OnUpdate(m_TimeStep);
            currentDemo->OnRender();
            ImGui::Begin("Demo");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if (currentDemo != demoMenu && ImGui::Button("<--")) {
                delete currentDemo;
                currentDemo = demoMenu;
            }
            currentDemo->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_WindowHandle);
        glfwPollEvents();

        float time = GetTime();
        m_FrameTime = time - m_LastFrameTime;
        m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
        m_LastFrameTime = time;
    }

    if (currentDemo != demoMenu)
        delete demoMenu;
    delete currentDemo;
}

float Application::GetTime()
{
    return (float)glfwGetTime();
}

int main()
{
    Application* app = new Application();
    app->Run();
}
