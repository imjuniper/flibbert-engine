#pragma once

#include "Flibbert/Core/ApplicationSubsystem.h"
#include "Flibbert/Core/ClassRegistry.h"

typedef union SDL_Event SDL_Event;

namespace Flibbert {

class Window;
class Renderer;

} // namespace Flibbert

namespace Flibbert::Modules::Imgui {

class ImguiSubsystem : public ApplicationSubsystem
{
	FBTCLASS(ImguiSubsystem, ApplicationSubsystem)

	using OnImguiRenderDelegate = MulticastDelegate<>;

public:
	virtual void Initialize(Application* application) override;
	virtual void OnUpdate(double ts) override;
	virtual void OnPostRender() override;
	virtual void Shutdown() override;

	void PreprocessWindowEvent(SDL_Event& event);

	OnImguiRenderDelegate OnImguiRender;
};

} // namespace Flibbert::Modules::Imgui
