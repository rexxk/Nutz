#include "nutz_pch.h"
#include "Window.h"

#include "Platform/GLFW/GLFWWindow.h"


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


}
