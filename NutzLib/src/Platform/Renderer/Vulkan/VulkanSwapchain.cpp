#include "nutz_pch.h"
#include "VulkanSwapchain.h"


#include "Core/Message/Messages.h"



namespace Nutz
{


	Ref<VulkanSwapchain> VulkanSwapchain::Create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, Ref<Window> window)
	{
		return CreateRef<VulkanSwapchain>(instance, physicalDevice, device, surface, window);
	}

	VulkanSwapchain::VulkanSwapchain(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, Ref<Window> window)
		: m_Instance(instance), m_PhysicalDevice(physicalDevice), m_Device(device), m_Surface(surface)
	{
		auto& windowProps = window->GetProperties();
		m_Width = windowProps.Width;
		m_Height = windowProps.Height;
		m_VSync = windowProps.VSync;

		FindSurfaceFormat();

		CreateSwapchain();


		MessageQueue::Subscribe(MessageType::WindowResized, [&](Ref<Nutz::Message> msg)
			{
				Ref<WindowResizedMessage> message = std::dynamic_pointer_cast<WindowResizedMessage>(msg);
				m_Width = message->Width();
				m_Height = message->Height();

				CreateSwapchain();

				return false;
			});
	}

	void VulkanSwapchain::Shutdown()
	{
		if (m_Swapchain != nullptr)
		{
			vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
			m_Swapchain = nullptr;
		}

		for (uint32_t i = 0; i < m_ImageCount; i++)
		{
			vkDestroyImageView(m_Device, m_Buffers[i].ImageView, nullptr);
			m_Buffers[i].ImageView = nullptr;
		}
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		LOG_CORE_TRACE("Create Vulkan swapchain - {},{}", m_Width, m_Height);

		VkSwapchainKHR oldSwapchain = m_Swapchain;


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
		createInfo.queueFamilyIndexCount = 0;
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


	VkResult VulkanSwapchain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex)
	{
		return vkAcquireNextImageKHR(m_Device, m_Swapchain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr, imageIndex);
	}

	VkResult VulkanSwapchain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &imageIndex;

		if (waitSemaphore)
		{
			presentInfo.pWaitSemaphores = &waitSemaphore;
			presentInfo.waitSemaphoreCount = 1;
		}

		return vkQueuePresentKHR(queue, &presentInfo);
	}

}
