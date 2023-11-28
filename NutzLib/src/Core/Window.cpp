#include "nutz_pch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"


namespace Nutz
{

	Ref<Window> Window::Create(const WindowProperties& props)
	{
#ifdef _WIN32
		return CreateRef<WindowsWindow>(props);
#endif
#ifdef __linux__
		return CreateRef<LinuxWindow>(props);
#endif		
	}


	Window::Window(const WindowProperties& props)
		: m_Properties(props)
	{

	}


}
