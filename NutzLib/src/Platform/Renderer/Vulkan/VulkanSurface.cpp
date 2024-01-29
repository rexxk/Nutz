#include "nutz_pch.h"
#include "VulkanSurface.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


namespace Nutz
{


	Ref<VulkanSurface> VulkanSurface::Create(VkInstance instance, void* windowHandle)
	{
		return CreateRef<VulkanSurface>(instance, windowHandle);
	}


	VulkanSurface::VulkanSurface(VkInstance instance, void* windowHandle)
	{

#ifdef _WIN32
		VkWin32SurfaceCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hinstance = GetModuleHandle(NULL);
		createInfo.hwnd = glfwGetWin32Window((GLFWwindow*)windowHandle);

		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &m_Surface) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Failed to create Vulkan win32 surface");
			return;
		}
#endif

#ifdef _LINUX_
		VkXlibSurfaceCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		createInfo.dpy = windowHandle;

		if (vkCreateXlibSurfaceKHR(instance, &createInfo, nullptr, &m_Surface) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Failed to create Vulkan XLib surface");
			return;
		}
#endif
	}

	VulkanSurface::~VulkanSurface()
	{

	}

	void VulkanSurface::Shutdown(VkInstance instance)
	{
		if (m_Surface != nullptr)
		{
			vkDestroySurfaceKHR(instance, m_Surface, nullptr);
			m_Surface = nullptr;
		}
	}

}
