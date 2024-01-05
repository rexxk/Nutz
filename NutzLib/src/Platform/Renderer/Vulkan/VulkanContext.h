#pragma once


#include "vulkan/vulkan.h"

#include "VulkanDevice.h"



namespace Nutz
{


	class VulkanContext
	{
	public:
		static Ref<VulkanContext> Create(void* windowHandle);

		VulkanContext(void* windowHandle);
		~VulkanContext();

		void Shutdown();

	private:
		bool CreateInstance();

	private:

		VkInstance m_Instance = nullptr;

		Ref<VulkanDevice> m_Device = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice = nullptr;


	};


}
