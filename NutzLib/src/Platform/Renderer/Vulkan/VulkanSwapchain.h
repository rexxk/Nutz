#pragma once

#include "VulkanContext.h"

#include "Renderer/Swapchain.h"

#include "Core/Window.h"

#include <vulkan/vulkan.h>



namespace Nutz
{


	struct SwapchainBuffer
	{
		VkImage Image;
		VkImageView ImageView;
	};


	class VulkanSwapchain : public Swapchain
	{
	public:
		static Ref<VulkanSwapchain> Create(const VulkanContextData& contextData, const WindowProperties& windowProperties);

		VulkanSwapchain(const VulkanContextData& contextData, const WindowProperties& windowProperties);

		virtual void Shutdown() override;

		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore);

		VkFormat GetFormat() { return m_Format; }
		VkRenderPass GetRenderPass() { return m_RenderPass; }

		VkCommandBuffer GetCurrentCommandBuffer() { return m_CommandBuffers[m_CurrentBufferIndex].CommandBuffer; }
		VkFramebuffer GetCurrentFramebuffer() { return m_Framebuffers[m_CurrentBufferIndex]; }

		VkExtent2D GetCurrentExtent() { return { m_Width, m_Height }; }

		virtual void BeginFrame() override;
		virtual void Present() override;

		static Ref<VulkanSwapchain> Get() { return std::dynamic_pointer_cast<VulkanSwapchain>(Swapchain::Get()); }

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

		uint32_t m_CurrentBufferIndex = 0;
		uint32_t m_CurrentImageIndex = 0;

		struct
		{
			VkSemaphore RenderComplete = nullptr;
			VkSemaphore PresentComplete = nullptr;
		} m_Semaphores;

		struct SwapchainCommandBuffer
		{
			VkCommandPool CommandPool = nullptr;
			VkCommandBuffer CommandBuffer = nullptr;	
		};

		VkRenderPass m_RenderPass = nullptr;

		std::vector<SwapchainCommandBuffer> m_CommandBuffers;

		std::vector<VkFence> m_WaitFences;
	};


}
