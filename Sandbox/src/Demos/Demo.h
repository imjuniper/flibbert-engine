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

		virtual void OnUpdate(float ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) {}
	};

	class DemoMenu : public Demo
	{
	public:
		explicit DemoMenu(Demo*& currentDemoPointer) : m_CurrentDemo(currentDemoPointer) {}

		void OnUpdate(float ts) override {}
		void OnRender() override {}
		void OnImGuiRender() override;

		template <typename T>
		void RegisterDemo(std::string_view name)
		{
			FBT_INFO("Registering demo {}", name);
			m_Demos.push_back(std::make_pair(name.data(), []() { return new T(); }));
		}

	private:
		Demo*& m_CurrentDemo;
		std::vector<std::pair<const char*, std::function<Demo*()>>> m_Demos;
	};
} // namespace Demo
