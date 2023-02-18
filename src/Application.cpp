#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <iostream>

#include "opengl/Renderer.h"

#include "demos/Demo.h"
#include "demos/DemoClearColor.h"
#include "demos/DemoTexture2D.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	/* Set OpenGL version to 4.1 on macOS */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	/* Create a windowed mode window and its src context */
	window = glfwCreateWindow(960, 540, "OpenGL Learning", nullptr, nullptr);

	if (!window)
		return -1;

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW Error" << std::endl;

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	GLCallV(glEnable(GL_BLEND));
	GLCallV(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Renderer renderer;

	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();

	Demo::Demo* currentDemo = nullptr;
	Demo::DemoMenu* demoMenu = new Demo::DemoMenu(currentDemo);
	currentDemo = demoMenu;

	demoMenu->RegisterDemo<Demo::DemoClearColor>("Clear Color");
	demoMenu->RegisterDemo<Demo::DemoTexture2D>("2D Texture");

	while (!glfwWindowShouldClose(window)) {
		GLCallV(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (currentDemo) {
			currentDemo->OnUpdate(0.0f);
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
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete currentDemo;
	if (currentDemo != demoMenu)
		delete demoMenu;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
