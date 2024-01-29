#pragma once


#include "Renderer/Framebuffer.h"

#include <vulkan/vulkan.h>


namespace Nutz
{


	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(const FramebufferSpecification& framebufferSpecification);

		virtual void Shutdown() override;

		void AttachImageViews(const std::vector<VkImageView>& imageViews);

		VkFramebuffer GetNextFramebuffer();

	private:
		FramebufferSpecification m_Specification;

		std::vector<VkFramebuffer> m_Framebuffers;

		uint32_t m_ActiveFramebuffer = 0;
	};


}
