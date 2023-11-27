#pragma once

#include "Core/Window.h"


#ifdef __linux

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
    private:

    }

#endif

}
