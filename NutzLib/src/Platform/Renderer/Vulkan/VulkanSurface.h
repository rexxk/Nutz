#pragma once

#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#ifdef _LINUX_
#define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>


namespace Nutz
{


	class VulkanSurface
	{
	public:
		static Ref<VulkanSurface> Create(VkInstance instance, void* windowHandle);

		VulkanSurface(VkInstance instance, void* windowHandle);
		~VulkanSurface();

		void Shutdown(VkInstance instance);

		VkSurfaceKHR Surface() { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface = nullptr;

	};


}
