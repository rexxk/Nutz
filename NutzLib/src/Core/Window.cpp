#include "nutz_pch.h"
#include "Window.h"

#include "Platform/GLFW/GLFWWindow.h"
#include "Platform/Renderer/Vulkan/VulkanSwapchain.h"

#include "Renderer/RendererContext.h"


namespace Nutz
{

	Ref<Window> Window::Create(const WindowProperties& props)
	{
		return CreateRef<GLFWWindow>(props);
	}


	Window::Window(const WindowProperties& props)
		: m_Properties(props)
	{

	}

	Window::~Window()
	{
		if (m_Swapchain != nullptr)
		{
			m_Swapchain->Shutdown();
		}

		if (m_RendererContext != nullptr)
		{
			m_RendererContext->Shutdown();
		}
	}

}
