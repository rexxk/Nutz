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

		VkDevice GetDevice() { return m_Device->GetVulkanDevice(); }

		Ref<VulkanSwapchain> GetSwapchain() { return m_Swapchain; }

	private:
		bool CreateInstance();

	private:

		VkInstance m_Instance = nullptr;

		Ref<VulkanDevice> m_Device = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice = nullptr;

		Ref<VulkanSurface> m_Surface = nullptr;

		Ref<VulkanSwapchain> m_Swapchain = nullptr;
	};


}
