#include "nutz_pch.h"
#include "VulkanContext.h"

#include "VulkanShader.h"


namespace Nutz
{


	
	static VulkanContextData s_ContextData;



	VulkanContext::VulkanContext()
	{
		if (!CreateInstance())
			return;

		s_ContextData.PhysicalDevice = VulkanPhysicalDevice::Select(s_ContextData.Instance);
		s_ContextData.Device = VulkanDevice::Create(s_ContextData.PhysicalDevice);
		s_ContextData.Surface = VulkanSurface::Create(s_ContextData.Instance);
	}

	VulkanContext::~VulkanContext()
	{

	}

//	VkDevice VulkanContext::GetDevice() { return s_ContextData.Device->GetVulkanDevice(); }

	VulkanContextData VulkanContext::GetContextData() { return s_ContextData; }

	void VulkanContext::Shutdown()
	{
//		if (!ShaderLibrary::IsEmpty())
//			ShaderLibrary::Shutdown();

		if (s_ContextData.Surface != nullptr)
			s_ContextData.Surface->Shutdown(s_ContextData.Instance);

		if (s_ContextData.Device != nullptr)
			s_ContextData.Device->Shutdown();

		if (s_ContextData.Instance != nullptr)
		{
			vkDestroyInstance(s_ContextData.Instance, nullptr);
			s_ContextData.Instance = nullptr;

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

		if (vkCreateInstance(&createInfo, nullptr, &s_ContextData.Instance) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Unable to create Vulkan instance object");
			return false;
		}

		LOG_CORE_TRACE("Vulkan instance object created");

		return true;
	}


}
