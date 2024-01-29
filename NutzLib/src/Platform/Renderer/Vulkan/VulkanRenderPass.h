#pragma once

#include "Renderer/RenderPass.h"

#include <vulkan/vulkan.h>


namespace Nutz
{


	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass();


	private:

		VkRenderPass m_RenderPass = nullptr;

	};



}
