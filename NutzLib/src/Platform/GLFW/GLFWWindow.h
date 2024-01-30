#pragma once

#include "Core/Window.h"

#include "GLFW/glfw3.h"


namespace Nutz
{

    class GLFWWindow : public Window
    {
    public:
        GLFWWindow(const WindowProperties& props);
		virtual ~GLFWWindow();

		virtual void HandleEvents() override;

        virtual void SetupRenderer() override;

        virtual void Present() override;

	private:
		virtual void CreateWindow() override;

    };


}
