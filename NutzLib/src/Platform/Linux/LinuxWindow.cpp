#include "nutz_pch.h"
#include "LinuxWindow.h"



namespace Nutz
{

#ifdef __linux__

    LinuxWindow::LinuxWindow(const WindowProperties& props)
        : Window(props)
    {
        LOG_CORE_TRACE("Creating Linux window");
    }

    LinuxWindow::~LinuxWindow()
    {

    }

	void LinuxWindow::HandleEvents()
    {

    }

	void LinuxWindow::CreateWindow()
    {

    }

#endif

}
