#pragma once

#include "VulkanContext.h"

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
		static Ref<VulkanSwapchain> Create(const VulkanContextData& contextData, const WindowProperties& windowProperties);

		VulkanSwapchain(const VulkanContextData& contextData, const WindowProperties& windowProperties);

		void Shutdown();

		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore);

		VkFormat GetFormat() { return m_Format; }

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
		std::vector<VkFramebuffer> m_Framebuffers;

		VkColorSpaceKHR m_ColorSpace;
		VkFormat m_Format;

		uint32_t m_Width = 0, m_Height = 0;
		bool m_VSync = false;

		uint32_t m_ImageCount = 0;
		std::vector<VkImage> m_Images;
		std::vector<SwapchainBuffer> m_Buffers;
	};


}
