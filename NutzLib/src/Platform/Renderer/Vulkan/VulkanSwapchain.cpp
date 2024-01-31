#include "nutz_pch.h"
#include "VulkanSwapchain.h"

#include "Core/Application.h"
#include "Core/Message/Messages.h"



namespace Nutz
{


	Ref<VulkanSwapchain> VulkanSwapchain::Create(const VulkanContextData& contextData, const WindowProperties& windowProperties)
	{
		return CreateRef<VulkanSwapchain>(contextData, windowProperties);
	}

	VulkanSwapchain::VulkanSwapchain(const VulkanContextData& contextData, const WindowProperties& windowProperties)
		: m_Instance(contextData.Instance), m_PhysicalDevice(contextData.PhysicalDevice->GetVulkanPhysicalDevice()), 
		  m_Device(contextData.Device->GetVulkanDevice()), m_Surface(contextData.Surface->Surface())
	{
		m_Width = windowProperties.Width;
		m_Height = windowProperties.Height;
		m_VSync = windowProperties.VSync;

		FindSurfaceFormat();

		CreateSwapchain();


		MessageQueue::Subscribe(MessageType::WindowResized, [&](Ref<Nutz::Message> msg)
			{
				Ref<WindowResizedMessage> message = std::dynamic_pointer_cast<WindowResizedMessage>(msg);

				if (message->Width() == 0 || message->Height() == 0)
					return false;

				m_Width = message->Width();
				m_Height = message->Height();

				VkDevice device = VulkanContext::Device();

				vkDeviceWaitIdle(device);
				CreateSwapchain();
				vkDeviceWaitIdle(device);

				return false;
			});
	}

	void VulkanSwapchain::Shutdown()
	{

		vkDeviceWaitIdle(m_Device);

		for (auto& fence : m_WaitFences)
		{
			if (fence != nullptr)
			{
				vkDestroyFence(m_Device, fence, nullptr);
				fence = nullptr;
			}
		}

		m_WaitFences.clear();

		if (m_RenderPass != nullptr)
		{
			vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
			m_RenderPass = nullptr;
		}

		if (m_Semaphores.PresentComplete != nullptr)
		{
			vkDestroySemaphore(m_Device, m_Semaphores.PresentComplete, nullptr);
			m_Semaphores.PresentComplete = nullptr;
		}

		if (m_Semaphores.RenderComplete != nullptr)
		{
			vkDestroySemaphore(m_Device, m_Semaphores.RenderComplete, nullptr);
			m_Semaphores.RenderComplete = nullptr;
		}

		for (auto& commandBuffer : m_CommandBuffers)
		{
			if (commandBuffer.CommandPool != nullptr)
			{
				vkDestroyCommandPool(m_Device, commandBuffer.CommandPool, nullptr);
				commandBuffer.CommandPool = nullptr;
			}
		}

		m_CommandBuffers.clear();

		for (auto& framebuffer : m_Framebuffers)
		{
			if (framebuffer != nullptr)
			{
				vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
				framebuffer = nullptr;
			}
		}

		m_Framebuffers.clear();

		for (uint32_t i = 0; i < (uint32_t)m_Buffers.size(); i++)
		{
			vkDestroyImageView(m_Device, m_Buffers[i].ImageView, nullptr);
			m_Buffers[i].ImageView = nullptr;
		}

		m_Buffers.clear();

		if (m_Swapchain != nullptr)
		{
			vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
			m_Swapchain = nullptr;
		}

		vkDeviceWaitIdle(m_Device);
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		LOG_CORE_TRACE("Create Vulkan swapchain - {},{}", m_Width, m_Height);

		VkSwapchainKHR oldSwapchain = m_Swapchain;

		vkDeviceWaitIdle(m_Device);

		for (auto& framebuffer : m_Framebuffers)
		{
			if (framebuffer != nullptr)
			{
				vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
				framebuffer = nullptr;
			}
		}

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, m_Surface, &surfaceCapabilities);

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, presentModes.data());

		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

		if (!m_VSync)
		{
			for (auto& actualPresentMode : presentModes)
			{
				if (actualPresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					presentMode = actualPresentMode;
					break;
				}

				if (actualPresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				{
					presentMode = actualPresentMode;
					break;
				}
			}
		}

		VkExtent2D swapchainExtent = {};

		if (surfaceCapabilities.currentExtent.width == (uint32_t)-1)
		{
			swapchainExtent.width = m_Width;
			swapchainExtent.height = m_Height;
		}
		else
		{
			swapchainExtent = surfaceCapabilities.currentExtent;
			m_Width = surfaceCapabilities.currentExtent.width;
			m_Height = surfaceCapabilities.currentExtent.height;
		}
		
		uint32_t desiredNumberOfSwapchainImages = surfaceCapabilities.minImageCount + 1;

		if ((surfaceCapabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfaceCapabilities.maxImageCount))
		{
			desiredNumberOfSwapchainImages = surfaceCapabilities.maxImageCount;
		}

		VkSurfaceTransformFlagsKHR pretransform;

		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			pretransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			pretransform = surfaceCapabilities.currentTransform;
		}

		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags =
		{
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};

		for (auto& actualCompositeAlpha : compositeAlphaFlags)
		{
			if (surfaceCapabilities.supportedCompositeAlpha & actualCompositeAlpha)
			{
				compositeAlpha = actualCompositeAlpha;
				break;
			}
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = desiredNumberOfSwapchainImages;
		createInfo.imageColorSpace = m_ColorSpace;
		createInfo.imageFormat = m_Format;
		createInfo.imageExtent = swapchainExtent;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)pretransform;
		createInfo.imageArrayLayers = 1;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = VulkanContext::GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
		createInfo.presentMode = presentMode;
		createInfo.oldSwapchain = oldSwapchain;
		createInfo.clipped = VK_TRUE;
		createInfo.compositeAlpha = compositeAlpha;

		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
		{
			createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		{
			createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		
		if (vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Unable to create Vulkan swapchain");
			return;
		}


		if (oldSwapchain)
		{
			for (uint32_t i = 0; i < m_ImageCount; i++)
			{
				vkDestroyImageView(m_Device, m_Buffers[i].ImageView, nullptr);
			}

			vkDestroySwapchainKHR(m_Device, oldSwapchain, nullptr);
		}

		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &m_ImageCount, nullptr);
		m_Images.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &m_ImageCount, m_Images.data());

		m_Buffers.resize(m_ImageCount);

		for (uint32_t i = 0; i < m_ImageCount; i++)
		{
			// Create color attachment views
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.format = m_Format;
			createInfo.components =
			{
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A,
			};
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

			m_Buffers[i].Image = m_Images[i];
			createInfo.image = m_Buffers[i].Image;

			if (vkCreateImageView(m_Device, &createInfo, nullptr, &m_Buffers[i].ImageView) != VK_SUCCESS)
			{
				LOG_CORE_ERROR("Unable to create swapchain image view");
				return;
			}
		}

		for (auto& commandBuffer : m_CommandBuffers)
		{
			vkDestroyCommandPool(m_Device, commandBuffer.CommandPool, nullptr);
		}

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = VulkanContext::GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = 1;

		m_CommandBuffers.resize(m_ImageCount);

		for (auto& commandBuffer : m_CommandBuffers)
		{
			vkCreateCommandPool(m_Device, &commandPoolCreateInfo, nullptr, &commandBuffer.CommandPool);

			commandBufferAllocateInfo.commandPool = commandBuffer.CommandPool;
			vkAllocateCommandBuffers(m_Device, &commandBufferAllocateInfo, &commandBuffer.CommandBuffer);
		}

		if (!m_Semaphores.PresentComplete || !m_Semaphores.RenderComplete)
		{
			VkSemaphoreCreateInfo semaphoreCreateInfo = {};
			semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_Semaphores.PresentComplete);
			vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_Semaphores.RenderComplete);
		}

		if (m_WaitFences.size() != m_ImageCount)
		{
			VkFenceCreateInfo fenceCreateInfo = {};
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			m_WaitFences.resize(m_ImageCount);

			for (auto& fence : m_WaitFences)
			{
				vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &fence);
			}
		}

		if (m_RenderPass != nullptr)
		{
			vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
			m_RenderPass = nullptr;
		}

		VkAttachmentDescription attachmentDescription = {};
		attachmentDescription.format = m_Format;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentReference = {};
		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentReference;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;
		subpassDescription.pResolveAttachments = nullptr;

		VkSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dstSubpass = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.srcAccessMask = 0;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = &attachmentDescription;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpassDescription;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &subpassDependency;

		if (vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Unable to create swapchain renderpass");
			return;
		}


		for (auto& framebuffer : m_Framebuffers)
		{
			vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
		}

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = m_RenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.width = m_Width;
		framebufferCreateInfo.height = m_Height;
		framebufferCreateInfo.layers = 1;

		m_Framebuffers.resize(m_ImageCount);

		for (uint32_t i = 0; i < m_ImageCount; i++)
		{
			framebufferCreateInfo.pAttachments = &m_Buffers[i].ImageView;
			vkCreateFramebuffer(m_Device, &framebufferCreateInfo, nullptr, &m_Framebuffers[i]);
		}

	}

	void VulkanSwapchain::FindSurfaceFormat()
	{
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, surfaceFormats.data());

		std::vector<VkFormat> preferredFormats =
		{
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_FORMAT_A8B8G8R8_UNORM_PACK32,
		};

		VkSurfaceFormatKHR selectedFormat = surfaceFormats[0];

		for (auto& availableFormat : surfaceFormats)
		{
			if (std::find(preferredFormats.begin(), preferredFormats.end(), availableFormat.format) != preferredFormats.end())
			{
				selectedFormat = availableFormat;
				break;
			}
		}

		m_ColorSpace = selectedFormat.colorSpace;
		m_Format = selectedFormat.format;
	}


	VkResult VulkanSwapchain::AcquireNextImage(VkSemaphore presentCompleteSemaphore)
	{
		return vkAcquireNextImageKHR(m_Device, m_Swapchain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr, &m_CurrentImageIndex);
	}

	void VulkanSwapchain::Present()
	{
		Ref<VulkanDevice> device = VulkanContext::GetVulkanDevice();

		const uint64_t FENCE_TIMEOUT = 1000000000;

		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		submitInfo.pWaitDstStageMask = &waitStageMask;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentBufferIndex].CommandBuffer;
		submitInfo.commandBufferCount = 1;

		vkResetFences(device->GetVulkanDevice(), 1, &m_WaitFences[m_CurrentBufferIndex]);
		vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, m_WaitFences[m_CurrentBufferIndex]);

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &m_CurrentImageIndex;

		presentInfo.pWaitSemaphores = &m_Semaphores.RenderComplete;
		presentInfo.waitSemaphoreCount = 1;

		VkResult result = vkQueuePresentKHR(device->GetGraphicsQueue(), &presentInfo);

		if (result != VK_SUCCESS)
		{
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				CreateSwapchain();
			}
		}

		if (++m_CurrentBufferIndex > 2)
			m_CurrentBufferIndex = 0;

		vkWaitForFences(device->GetVulkanDevice(), 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX);

	}

	void VulkanSwapchain::BeginFrame()
	{
		AcquireNextImage(m_Semaphores.PresentComplete);

		vkResetCommandPool(m_Device, m_CommandBuffers[m_CurrentBufferIndex].CommandPool, 0);





	}

}
