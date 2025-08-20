#pragma once

#include "Flibbert/Debug/Profiling.h"

#include <Flibbert.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace Demo
{
	struct PerFrameUniformData
	{
		glm::mat4 View;
		glm::mat4 Projection;
		glm::vec3 ViewPosition;
		// Lights?
	};

	struct PerObjectUniformData
	{
		glm::mat4 Transform;
		// Probably also material properties?
	};

	class Demo
	{
	public:
		Demo() = default;
		virtual ~Demo() = default;

		virtual void OnUpdate(float ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) {}

		virtual const char* GetName() const = 0;
	};

	class DemoMenu : public Demo
	{
	public:
		explicit DemoMenu(Demo*& currentDemoPointer)
		    : m_Renderer(Flibbert::Renderer::Get()), m_CurrentDemo(currentDemoPointer)
		{
		}

		void OnUpdate(float ts) override;
		void OnRender() override {}
		void OnImGuiRender() override;

		const char* GetName() const override { return Name; }

		template <typename T>
		void RegisterDemo()
		{
			ZoneScoped;

			FBT_INFO("Registering demo {}", T::Name);
			m_Demos.push_back(std::make_pair(T::Name, []() { return new T(); }));
		}

	public:
		static constexpr auto Name = "Demo Menu";

	private:
		Flibbert::Renderer& m_Renderer;

		Demo*& m_CurrentDemo;
		std::vector<std::pair<const char*, std::function<Demo*()>>> m_Demos;
	};
} // namespace Demo
