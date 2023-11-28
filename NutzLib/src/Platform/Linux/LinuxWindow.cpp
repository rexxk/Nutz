#include "nutz_pch.h"
#include "LinuxWindow.h"


namespace Nutz
{

#ifdef __linux__

    LinuxWindow::LinuxWindow(const WindowProperties& props)
        : Window(props)
    {
        LOG_CORE_TRACE("Creating Linux window");

        CreateWindow();
    }

    LinuxWindow::~LinuxWindow()
    {

    }

	void LinuxWindow::HandleEvents()
    {


    }

	void LinuxWindow::CreateWindow()
    {
        glfwInit();

        glfwCreateWindow(1280, 720, "Test title", nullptr, nullptr);



    }

#endif

}
