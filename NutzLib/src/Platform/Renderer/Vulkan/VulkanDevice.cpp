#include "nutz_pch.h"
#include "VulkanDevice.h"




namespace Nutz
{

	Ref<VulkanDevice> VulkanDevice::Create(Ref<VulkanPhysicalDevice> physicalDevice)
	{
		return CreateRef<VulkanDevice>(physicalDevice);
	}


	VulkanDevice::VulkanDevice(Ref<VulkanPhysicalDevice> physicalDevice)
	{
		std::vector<const char*> deviceExtensions;

		if (!physicalDevice->IsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
		{
			LOG_CORE_ERROR("Vulkan Swapchain extension is not supported.");
			return;
		}

		deviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = (uint32_t)physicalDevice->m_QueueCreateInfos.size();
		createInfo.pQueueCreateInfos = physicalDevice->m_QueueCreateInfos.data();
		createInfo.pEnabledFeatures = &physicalDevice->m_Features;

		if ((uint32_t)deviceExtensions.size() > 0)
		{
			createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
			createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		}

		if (vkCreateDevice(physicalDevice->GetVulkanPhysicalDevice(), &createInfo, nullptr, &m_Device) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Unable to create logical Vulkan device");
			return;
		}

		vkGetDeviceQueue(m_Device, physicalDevice->m_QueueFamilyIndices.Graphics, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, physicalDevice->m_QueueFamilyIndices.Compute, 0, &m_ComputeQueue);
	}

	VulkanDevice::~VulkanDevice()
	{
		Shutdown();
	}

	void VulkanDevice::Shutdown()
	{
		if (m_Device != nullptr)
		{
			vkDestroyDevice(m_Device, nullptr);
			m_Device = nullptr;
		}
	}






	Ref<VulkanPhysicalDevice> VulkanPhysicalDevice::Select(VkInstance instance)
	{
		return CreateRef<VulkanPhysicalDevice>(instance);
	}


	VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance instance)
	{
		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
		vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());

		if (gpuCount == 0)
		{
			LOG_CORE_ERROR("No discrete GPU supporting Vulkan is available");
		}

		LOG_CORE_TRACE("{} discrete GPU(s) supporting Vulkan was found", gpuCount);

		VkPhysicalDevice selectedDevice = nullptr;

		for (auto& physicalDevice : physicalDevices)
		{
			vkGetPhysicalDeviceProperties(physicalDevice, &m_Properties);

			if (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				selectedDevice = physicalDevice;
				LOG_CORE_TRACE("Discrete GPU selected: {}", m_Properties.deviceName);
				break;
			}
		}

		if (!selectedDevice)
		{
			for (auto& physicalDevice : physicalDevices)
			{
				vkGetPhysicalDeviceProperties(physicalDevice, &m_Properties);

				if (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
				{
					selectedDevice = physicalDevice;
					LOG_CORE_TRACE("Integrated GPU selected: {}", m_Properties.deviceName);
					break;
				}
			}

		}

		if (!selectedDevice)
		{
			LOG_CORE_ERROR("No physical Vulkan device found");
			selectedDevice = physicalDevices.back();
		}

		m_PhysicalDevice = selectedDevice;

		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		m_QueueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());

		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);

		if (extensionCount > 0)
		{
			std::vector<VkExtensionProperties> extensions(extensionCount);

			if (vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, &extensions.front()) == VK_SUCCESS)
			{
				LOG_CORE_TRACE("Vulkan device has {} extensions", (uint32_t)extensions.size());

				for (auto& extension : extensions)
				{
					m_SupportedExtensions.emplace_back(extension.extensionName);
					LOG_CORE_TRACE(" - {}", extension.extensionName);
				}
			}
		}

		int requestedQueueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT;

		m_QueueFamilyIndices = GetQueueFamilyIndices(requestedQueueFlags);

		static const float defaultQueuePriority = 0.0f;

		if (requestedQueueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;

			m_QueueCreateInfos.emplace_back(queueInfo);
		}

		if (requestedQueueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			if (m_QueueFamilyIndices.Compute != m_QueueFamilyIndices.Graphics)
			{
				VkDeviceQueueCreateInfo queueInfo = {};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultQueuePriority;

				m_QueueCreateInfos.emplace_back(queueInfo);
			}
		}

		if (requestedQueueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			if ((m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Compute) && (m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Graphics))
			{
				VkDeviceQueueCreateInfo queueInfo = {};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Transfer;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultQueuePriority;

				m_QueueCreateInfos.emplace_back(queueInfo);
			}
		}

		m_DepthFormat = FindDepthFormat();
	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{

	}

	VulkanPhysicalDevice::QueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilyIndices(int queueFlags)
	{
		QueueFamilyIndices indices;

		if (queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			for (uint32_t i = 0; i < (uint32_t)m_QueueFamilyProperties.size(); i++)
			{
				auto& queueFamilyProperty = m_QueueFamilyProperties[i];

				if ((queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
				{
					indices.Compute = i;
					break;
				}
			}
		}

		if (queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			for (uint32_t i = 0; i < (uint32_t)m_QueueFamilyProperties.size(); i++)
			{
				auto& queueFamilyProperty = m_QueueFamilyProperties[i];

				if ((queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)
				{
					indices.Transfer = i;
					break;
				}
			}
		}

		for (uint32_t i = 0; i < (uint32_t)m_QueueFamilyProperties.size(); i++)
		{
			if ((queueFlags & VK_QUEUE_TRANSFER_BIT) && indices.Transfer == -1)
			{
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
				{
					indices.Transfer = i;
				}
			}

			if ((queueFlags & VK_QUEUE_COMPUTE_BIT) && indices.Compute == -1)
			{
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
				{
					indices.Compute = i;
				}
			}

			if ((queueFlags & VK_QUEUE_GRAPHICS_BIT) && indices.Graphics == -1)
			{
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.Graphics = i;
				}
			}
		}

		return indices;
	}

	VkFormat VulkanPhysicalDevice::FindDepthFormat()
	{
		std::vector<VkFormat> depthFormats =
		{
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM,
		};

		for (auto& format : depthFormats)
		{
			VkFormatProperties formatProperties;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &formatProperties);

			if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				return format;
		}

		return VK_FORMAT_UNDEFINED;
	}

	bool VulkanPhysicalDevice::IsExtensionSupported(const std::string& extensionName) const
	{
		if (std::find(m_SupportedExtensions.begin(), m_SupportedExtensions.end(), extensionName) != m_SupportedExtensions.end())
		{
			return true;
		}

		return false;
	}

}
