#include "nutz_pch.h"
#include "VulkanRenderPass.h"

#include "VulkanContext.h"

#include "Core/Application.h"


namespace Nutz
{


	VkFormat FormatTypeToVulkanFormat(FormatType formatType)
	{
		switch (formatType)
		{
			case FormatType::B8G8R8A8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
			case FormatType::R8G8B8A8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
		}

		return VK_FORMAT_UNDEFINED;
	}

	VkSampleCountFlagBits SampleCountTypeToVulkanSampleCount(SampleCountType sampleCountType)
	{
		switch (sampleCountType)
		{
			case SampleCountType::Samples_1: return VK_SAMPLE_COUNT_1_BIT;
			case SampleCountType::Samples_2: return VK_SAMPLE_COUNT_2_BIT;
			case SampleCountType::Samples_4: return VK_SAMPLE_COUNT_4_BIT;
			case SampleCountType::Samples_8: return VK_SAMPLE_COUNT_8_BIT;
			case SampleCountType::Samples_16: return VK_SAMPLE_COUNT_16_BIT;
			case SampleCountType::Samples_32: return VK_SAMPLE_COUNT_32_BIT;
			case SampleCountType::Samples_64: return VK_SAMPLE_COUNT_64_BIT;
		}

		return VK_SAMPLE_COUNT_1_BIT;
	}

	VkAttachmentLoadOp AttachmentLoadOpTypeToVulkanAttachmentLoadOp(AttachmentLoadOpType attachmentLoadOpType)
	{
		switch (attachmentLoadOpType)
		{
			case AttachmentLoadOpType::Clear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case AttachmentLoadOpType::Load: return VK_ATTACHMENT_LOAD_OP_LOAD;
			case AttachmentLoadOpType::DontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}

		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	}

	VkAttachmentStoreOp AttachmentStoreOpTypeToVulkanAttachmentStoreOp(AttachmentStoreOpType attachmentStoreOpType)
	{
		switch (attachmentStoreOpType)
		{
			case AttachmentStoreOpType::None: return VK_ATTACHMENT_STORE_OP_NONE;
			case AttachmentStoreOpType::Store: return VK_ATTACHMENT_STORE_OP_STORE;
			case AttachmentStoreOpType::DontCare: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}

		return VK_ATTACHMENT_STORE_OP_DONT_CARE;
	}


	VulkanRenderPass::VulkanRenderPass(const RenderPassSpecification& renderPassSpecification)
		: m_Specification(renderPassSpecification)
	{
		VkDevice device = std::dynamic_pointer_cast<VulkanContext>(Application::Get().GetWindow()->GetRendererContext())->GetDevice();

		VkAttachmentDescription colorAttachmentDescription = {};
		colorAttachmentDescription.format = FormatTypeToVulkanFormat(renderPassSpecification.Format);
		colorAttachmentDescription.samples = SampleCountTypeToVulkanSampleCount(renderPassSpecification.Samples);
		colorAttachmentDescription.loadOp = AttachmentLoadOpTypeToVulkanAttachmentLoadOp(renderPassSpecification.AttachmentLoadOp);
		colorAttachmentDescription.storeOp = AttachmentStoreOpTypeToVulkanAttachmentStoreOp(renderPassSpecification.AttachmentStoreOp);
		colorAttachmentDescription.stencilLoadOp = AttachmentLoadOpTypeToVulkanAttachmentLoadOp(renderPassSpecification.StencilLoadOp);
		colorAttachmentDescription.stencilStoreOp = AttachmentStoreOpTypeToVulkanAttachmentStoreOp(renderPassSpecification.StencilStoreOp);
		colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentReference = {};
		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentReference;

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpassDescription;

		if (vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Failed to create renderpass");
			return;
		}

	}

	void VulkanRenderPass::Shutdown()
	{
		VkDevice device = std::dynamic_pointer_cast<VulkanContext>(Application::Get().GetWindow()->GetRendererContext())->GetDevice();

		if (m_RenderPass != nullptr)
		{
			vkDestroyRenderPass(device, m_RenderPass, nullptr);
			m_RenderPass = nullptr;
		}
	}


}
