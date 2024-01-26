#include "nutz_pch.h"
#include "VulkanContext.h"



namespace Nutz
{


	VulkanContext::VulkanContext()
	{
		if (!CreateInstance())
			return;

		m_PhysicalDevice = VulkanPhysicalDevice::Select(m_Instance);

		m_Device = VulkanDevice::Create(m_PhysicalDevice);

		m_Surface = VulkanSurface::Create(m_Instance);

		m_Swapchain = VulkanSwapchain::Create(m_Instance, m_PhysicalDevice->GetVulkanPhysicalDevice(), m_Device->GetVulkanDevice(), m_Surface->Surface());
	}

	VulkanContext::~VulkanContext()
	{
		Shutdown();
	}

	void VulkanContext::Shutdown()
	{
		if (m_Swapchain != nullptr)
			m_Swapchain->Shutdown();

		if (m_Surface != nullptr)
			m_Surface->Shutdown(m_Instance);

		if (m_Device != nullptr)
			m_Device->Shutdown();

		if (m_Instance != nullptr)
		{
			vkDestroyInstance(m_Instance, nullptr);
			m_Instance = nullptr;

			LOG_CORE_TRACE("Vulkan instance object destroyed");
		}
	}

	bool VulkanContext::CreateInstance()
	{
		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.apiVersion = VK_API_VERSION_1_3;
		applicationInfo.pEngineName = "Nutz";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pApplicationName = "NutzApplication";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		std::vector<const char*> instanceLayers;
		std::vector<const char*> instanceExtensions;

		instanceExtensions.emplace_back("VK_KHR_surface");

#ifdef DEBUG
		instanceLayers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif
#ifdef _WIN32
		instanceExtensions.emplace_back("VK_KHR_win32_surface");
#endif


		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &applicationInfo;
		
		createInfo.enabledLayerCount = (uint32_t)instanceLayers.size();
		createInfo.ppEnabledLayerNames = instanceLayers.data();

		createInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		createInfo.ppEnabledExtensionNames = instanceExtensions.data();

		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Unable to create Vulkan instance object");
			return false;
		}

		LOG_CORE_TRACE("Vulkan instance object created");

		return true;
	}


}
