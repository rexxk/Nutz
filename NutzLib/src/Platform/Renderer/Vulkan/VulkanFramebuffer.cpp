#include "nutz_pch.h"
#include "VulkanFramebuffer.h"

#include "VulkanContext.h"
#include "VulkanRenderPass.h"

#include "Core/Application.h"


namespace Nutz
{


	VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification& framebufferSpecification)
		: m_Specification(framebufferSpecification)
	{

	}


	void VulkanFramebuffer::Shutdown()
	{
		VkDevice device = VulkanContext::Device();

		for (auto& framebuffer : m_Framebuffers)
		{
			if (framebuffer != nullptr)
			{
				vkDestroyFramebuffer(device, framebuffer, nullptr);
			}
		}

		m_Framebuffers.clear();
	}

	void VulkanFramebuffer::AttachImageViews(const std::vector<VkImageView>& imageViews)
	{
		VkDevice device = VulkanContext::Device();

		for (auto& imageView : imageViews)
		{
			VkFramebufferCreateInfo framebufferCreateInfo = {};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.width = m_Specification.Width;
			framebufferCreateInfo.height = m_Specification.Height;

			framebufferCreateInfo.layers = 1;

			framebufferCreateInfo.renderPass = std::dynamic_pointer_cast<VulkanRenderPass>(m_Specification.RenderPass)->GetRenderPass();

			std::vector<VkImageView> framebufferImageViews = { imageView };

			framebufferCreateInfo.attachmentCount = (uint32_t)framebufferImageViews.size();
			framebufferCreateInfo.pAttachments = framebufferImageViews.data();

			VkFramebuffer framebuffer;

			if (vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffer) != VK_SUCCESS)
			{
				LOG_CORE_ERROR("Failed to create framebuffer");
				return;
			}

			m_Framebuffers.push_back(framebuffer);

			LOG_CORE_TRACE("Framebuffer created");
		}
	}

	VkFramebuffer VulkanFramebuffer::GetNextFramebuffer()
	{
		if (m_ActiveFramebuffer >= (uint32_t)m_Framebuffers.size())
		{
			m_ActiveFramebuffer = 0;
		}

		return m_Framebuffers[m_ActiveFramebuffer];
	}

}
