#pragma once

#include "Flibbert/Core/ApplicationSubsystem.h"
#include "Flibbert/Core/ClassRegistry.h"

namespace Flibbert {

class Window;
class Renderer;
class InputEvent;

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

	void PreprocessInputEvent(const std::shared_ptr<InputEvent>& event);
	void HandleWindowFocusedGained(Window& window);
	void HandleWindowFocusedLost(Window& window);

	OnImguiRenderDelegate OnImguiRender;
};

} // namespace Flibbert::Modules::Imgui
