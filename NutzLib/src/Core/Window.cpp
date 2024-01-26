#include "nutz_pch.h"
#include "Window.h"

#include "Platform/GLFW/GLFWWindow.h"

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

	void Window::CreateRendererContext()
	{
		m_RendererContext = RendererContext::Create();
	}
}
