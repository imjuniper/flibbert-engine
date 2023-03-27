#pragma once

#include <Flibbert.h>

#include <imgui.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace Demo {

	class Demo {
	public:
		Demo() = default;
		virtual ~Demo() = default;

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class DemoMenu : public Demo {
	public:
		explicit DemoMenu(Demo*& currentDemoPointer);

		void OnImGuiRender() override;

		template <typename T>
		void RegisterDemo(const std::string& name) {
			FBT_INFO("Registering demo {}", name);
			m_Demos.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Demo*& m_CurrentDemo;
		std::vector<std::pair<std::string, std::function<Demo*()>>> m_Demos;
	};

} // namespace Demo
