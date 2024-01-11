#pragma once


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
		static Ref<VulkanSwapchain> Create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height);

		VulkanSwapchain(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height);

		void Shutdown();

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
	};


}
