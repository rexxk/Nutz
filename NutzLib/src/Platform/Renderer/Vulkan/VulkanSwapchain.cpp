#include "nutz_pch.h"
#include "VulkanSwapchain.h"


#include "Core/Message/Messages.h"



namespace Nutz
{


	Ref<VulkanSwapchain> VulkanSwapchain::Create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height, bool vsync)
	{
		return CreateRef<VulkanSwapchain>(instance, physicalDevice, device, surface, width, height, vsync);
	}

	VulkanSwapchain::VulkanSwapchain(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height, bool vsync)
		: m_Instance(instance), m_PhysicalDevice(physicalDevice), m_Device(device), m_Surface(surface), m_Width(width), m_Height(height), m_VSync(vsync)
	{
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

}
