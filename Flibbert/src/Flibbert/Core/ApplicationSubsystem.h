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

	virtual void Shutdown() {}

protected:
	// @todo make this weak ptr?
	Application* m_Application;
};

} // namespace Flibbert
