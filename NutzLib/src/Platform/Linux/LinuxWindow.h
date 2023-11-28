#pragma once

#include "Core/Window.h"


#ifdef __linux__

#include "GLFW/glfw3.h"


namespace Nutz
{

    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowProperties& props);
		virtual ~LinuxWindow();

		virtual void HandleEvents() override;

	private:
		virtual void CreateWindow() override;

    };

#endif

}
