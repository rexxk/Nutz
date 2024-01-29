#pragma once


#include "vulkan/vulkan.h"

#include "Renderer/RendererContext.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"

#include "Core/Window.h"

namespace Nutz
{


	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext();
		~VulkanContext();

		virtual void Shutdown() override;

		//VkDevice GetDevice() { return m_Device->GetVulkanDevice(); }
		VkDevice GetDevice();

//		Ref<VulkanSwapchain> GetSwapchain() { return m_Swapchain; }
		Ref<VulkanSwapchain> GetSwapchain();

	private:
		bool CreateInstance();

	};


}
