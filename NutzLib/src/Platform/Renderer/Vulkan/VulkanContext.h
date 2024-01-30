#pragma once

#include "VulkanDevice.h"
#include "VulkanSurface.h"

#include "Renderer/RendererContext.h"

#include "Core/Window.h"

#include <vulkan/vulkan.h>


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
		VulkanContext();
		~VulkanContext();

		virtual void Shutdown() override;

		//VkDevice GetDevice() { return m_Device->GetVulkanDevice(); }
//		VkDevice GetDevice();

		VulkanContextData GetContextData();

//		Ref<VulkanSwapchain> GetSwapchain() { return m_Swapchain; }

		static Ref<VulkanContext> Get() { return std::dynamic_pointer_cast<VulkanContext>(Renderer::GetContext()); }
		static VkDevice Device() { return Get()->GetContextData().Device->GetVulkanDevice(); }
		static Ref<VulkanDevice> GetVulkanDevice() { return Get()->GetContextData().Device; }

	private:
		bool CreateInstance();

	};


}
