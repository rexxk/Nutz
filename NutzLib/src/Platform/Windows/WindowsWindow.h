#pragma once

#include "Core/Window.h"


#ifdef _WIN32

namespace Nutz
{

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		virtual void HandleEvents() override;

	private:
		virtual void CreateWindow() override;
    private:

    }


}

#endif
