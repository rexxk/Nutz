#pragma once

#include "Renderer/RenderPass.h"

#include <vulkan/vulkan.h>


namespace Nutz
{


	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(const RenderPassSpecification& renderPassSpecification);

		virtual void Shutdown() override;

		VkRenderPass GetRenderPass() { return m_RenderPass; }

	private:

		VkRenderPass m_RenderPass = nullptr;

		RenderPassSpecification m_Specification;

	};



}
