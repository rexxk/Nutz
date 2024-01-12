#pragma once

#include "Core/Window.h"

#include <vulkan/vulkan.h>



namespace Nutz
{


	struct SwapchainBuffer
	{
		VkImage Image;
		VkImageView ImageView;
	};


	class VulkanSwapchain
	{
	public:
		static Ref<VulkanSwapchain> Create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, Ref<Window> window);

		VulkanSwapchain(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, Ref<Window> window);

		void Shutdown();

		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore);

	private:
		void CreateSwapchain();

		void FindSurfaceFormat();

	private:
		VkInstance m_Instance = nullptr;
		VkPhysicalDevice m_PhysicalDevice = nullptr;
		VkDevice m_Device = nullptr;
		VkSurfaceKHR m_Surface = nullptr;

		VkSwapchainKHR m_Swapchain = nullptr;

		std::vector<SwapchainBuffer> m_SwapchainBuffers;

		VkColorSpaceKHR m_ColorSpace;
		VkFormat m_Format;

		uint32_t m_Width = 0, m_Height = 0;
		bool m_VSync = false;

		uint32_t m_ImageCount = 0;
		std::vector<VkImage> m_Images;
		std::vector<SwapchainBuffer> m_Buffers;
	};


}
