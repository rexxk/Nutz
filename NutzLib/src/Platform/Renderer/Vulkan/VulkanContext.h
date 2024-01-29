#pragma once


#include "vulkan/vulkan.h"

#include "Renderer/RendererContext.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"

#include "Core/Window.h"

namespace Nutz
{

	struct VulkanContextData
	{
		VkInstance Instance = nullptr;
		Ref<VulkanPhysicalDevice> PhysicalDevice = nullptr;
		Ref<VulkanDevice> Device = nullptr;

		Ref<VulkanSurface> Surface = nullptr;
	};

	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext(void* windowHandle);
		~VulkanContext();

		virtual void Shutdown() override;

		//VkDevice GetDevice() { return m_Device->GetVulkanDevice(); }
		VkDevice GetDevice();

		VulkanContextData GetContextData();

//		Ref<VulkanSwapchain> GetSwapchain() { return m_Swapchain; }

	private:
		bool CreateInstance();

	};


}
