#include "nutz_pch.h"
#include "LinuxWindow.h"

#include "Core/Message/Messages.h"


namespace Nutz
{

#ifdef __linux__

    LinuxWindow::LinuxWindow(const WindowProperties& props)
        : Window(props)
    {
        LOG_CORE_TRACE("Creating Linux window");

        CreateWindow();

        
        glfwSetWindowCloseCallback((GLFWwindow*)m_Handle, [](GLFWwindow* window)
        {
            Ref<Message> message = CreateRef<WindowClosedMessage>((void*)window);
            MessageQueue::Add(message);
        });

        glfwSetWindowSizeCallback((GLFWwindow*)m_Handle, [](GLFWwindow* window, int width, int height)
        {
            Ref<Message> message = CreateRef<WindowResizedMessage>((void*)window, (uint32_t)width, (uint32_t)height);
            MessageQueue::Add(message);
        });

    }

    LinuxWindow::~LinuxWindow()
    {

    }

	void LinuxWindow::HandleEvents()
    {
        glfwPollEvents();

    }

	void LinuxWindow::CreateWindow()
    {
        glfwInit();

        m_Handle = glfwCreateWindow(1280, 720, "Test title", nullptr, nullptr);



    }

#endif

}
