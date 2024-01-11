#include "nutz_pch.h"
#include "VulkanSwapchain.h"


#include "Core/Message/Messages.h"



namespace Nutz
{


	Ref<VulkanSwapchain> VulkanSwapchain::Create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height)
	{
		return CreateRef<VulkanSwapchain>(instance, physicalDevice, device, surface, width, height);
	}

	VulkanSwapchain::VulkanSwapchain(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height)
		: m_Instance(instance), m_PhysicalDevice(physicalDevice), m_Device(device), m_Surface(surface), m_Width(width), m_Height(height)
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
