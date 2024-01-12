#include "nutz_pch.h"
#include "GLFWWindow.h"

#include "Core/Message/Messages.h"


namespace Nutz
{

    GLFWWindow::GLFWWindow(const WindowProperties& props)
        : Window(props)
    {
        LOG_CORE_TRACE("Creating GLFW window");

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

        glfwSetKeyCallback((GLFWwindow*)m_Handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                Ref<Message> message = CreateRef<KeyPressedMessage>(key, scancode);
                MessageQueue::Add(message);
            }
            if (action == GLFW_RELEASE)
            {
                Ref<Message> message = CreateRef<KeyReleasedMessage>(key, scancode);
                MessageQueue::Add(message);
            }
            
        });

        glfwSetMouseButtonCallback((GLFWwindow*)m_Handle, [](GLFWwindow* window, int button, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                Ref<Message> message = CreateRef<MouseButtonPressedMessage>(button);
                MessageQueue::Add(message);
            }
            if (action == GLFW_RELEASE)
            {
                Ref<Message> message = CreateRef<MouseButtonReleasedMessage>(button);
                MessageQueue::Add(message);
            }
        });

    }

    GLFWWindow::~GLFWWindow()
    {

    }

	void GLFWWindow::HandleEvents()
    {
        glfwPollEvents();

    }

	void GLFWWindow::CreateWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Handle = glfwCreateWindow(m_Properties.Width, m_Properties.Height, m_Properties.Title.c_str(), nullptr, nullptr);

        if (m_Properties.Mode == WindowMode::Maximized)
        {
            glfwMaximizeWindow((GLFWwindow*)m_Handle);
        }

    }

}
