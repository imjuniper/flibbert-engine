#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Core/ClassRegistry.h"

namespace Flibbert {

class Application;

class ApplicationSubsystem
{
	FBTBASECLASS(ApplicationSubsystem)

public:
	ApplicationSubsystem() = default;

	virtual void Initialize(Application* application)
	{
		m_Application = application;
	}

	virtual void OnUpdate(double ts) {}
	virtual void OnRender() {}
	virtual void OnPostRender() {}

	virtual void Shutdown() {}

	[[nodiscard]] Application* GetApplication() const
	{
		return m_Application;
	}

private:
	// @todo make this weak ptr?
	Application* m_Application;
};

} // namespace Flibbert
