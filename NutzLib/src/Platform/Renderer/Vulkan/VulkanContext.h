#pragma once


#include "vulkan/vulkan.h"

#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"


namespace Nutz
{


	class VulkanContext
	{
	public:
		static Ref<VulkanContext> Create(void* windowHandle, uint32_t width, uint32_t height);

		VulkanContext(void* windowHandle, uint32_t width, uint32_t height);
		~VulkanContext();

		void Shutdown();

	private:
		bool CreateInstance();

	private:

		VkInstance m_Instance = nullptr;

		Ref<VulkanDevice> m_Device = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice = nullptr;

		Ref<VulkanSurface> m_Surface = nullptr;

		Ref<VulkanSwapchain> m_Swapchain = nullptr;
	};


}
