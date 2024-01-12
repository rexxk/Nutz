#pragma once


#include "vulkan/vulkan.h"

#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"

#include "Core/Window.h"

namespace Nutz
{


	class VulkanContext
	{
	public:
		static Ref<VulkanContext> Create(Ref<Window> window);

		VulkanContext(Ref<Window> window);
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
