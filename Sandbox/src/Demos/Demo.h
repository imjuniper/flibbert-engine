#pragma once

#include <Flibbert.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace Demo
{
	class Demo
	{
	public:
		Demo() = default;
		virtual ~Demo() = default;

		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
	};

	class DemoMenu : public Demo
	{
	public:
		explicit DemoMenu(Demo*& currentDemoPointer) : m_CurrentDemo(currentDemoPointer) {}

		void OnUpdate(float deltaTime) override {}
		void OnRender() override {}
		void OnImGuiRender() override;

		template <typename T>
		void RegisterDemo(const std::string& name)
		{
			FBT_INFO("Registering demo {}", name);
			m_Demos.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Demo*& m_CurrentDemo;
		std::vector<std::pair<std::string, std::function<Demo*()>>> m_Demos;
	};
} // namespace Demo
