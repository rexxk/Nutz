#include "nutz_pch.h"
#include "VulkanPipeline.h"
#include "VulkanShader.h"

#include "Core/Application.h"

#include "VulkanContext.h"

namespace Nutz
{

	VkPrimitiveTopology PrimitiveTopologyTypeToVulkan(PrimitiveTopologyType type)
	{
		switch (type)
		{
			case PrimitiveTopologyType::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case PrimitiveTopologyType::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case PrimitiveTopologyType::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case PrimitiveTopologyType::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case PrimitiveTopologyType::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case PrimitiveTopologyType::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		}

		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}

	VkPolygonMode PolygonFillTypeToVulkan(PolygonFillType type)
	{
		switch (type)
		{
			case PolygonFillType::Wireframe: return VK_POLYGON_MODE_LINE;
			case PolygonFillType::Solid: return VK_POLYGON_MODE_FILL;
		}

		return VK_POLYGON_MODE_FILL;
	}

	VkShaderStageFlagBits ShaderDomainToShaderStageFlag(ShaderDomain shaderDomain)
	{
		switch (shaderDomain)
		{
			case ShaderDomain::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderDomain::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderDomain::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		}

		return VK_SHADER_STAGE_VERTEX_BIT;
	}

	VulkanPipeline::VulkanPipeline(const PipelineProperties& pipelineProperties)
		: m_Properties(pipelineProperties)
	{
		CreatePipeline();

	}

	void VulkanPipeline::Shutdown()
	{
		VkDevice device = std::dynamic_pointer_cast<VulkanContext>(Application::Get().GetWindow()->GetRendererContext())->GetDevice();

		if (m_RenderPass != nullptr)
		{
			vkDestroyRenderPass(device, m_RenderPass, nullptr);
			m_RenderPass = nullptr;
		}

		if (m_PipelineLayout != nullptr)
		{
			vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
			m_PipelineLayout = nullptr;
		}

		if (m_Pipeline != nullptr)
		{
			vkDestroyPipeline(device, m_Pipeline, nullptr);
			m_Pipeline = nullptr;
		}

	}

	void VulkanPipeline::CreatePipeline()
	{
		WindowProperties& windowProperties = Application::Get().GetWindow()->GetProperties();

		Ref<VulkanContext> context = std::dynamic_pointer_cast<VulkanContext>(Application::Get().GetWindow()->GetRendererContext());
		VkDevice device = context->GetDevice();
		Ref<VulkanSwapchain> swapchain = context->GetSwapchain();

		Ref<VulkanShader> shader = std::dynamic_pointer_cast<VulkanShader>(m_Properties.Shader);

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		std::vector<VkDynamicState> dynamicStates =
		{
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_VIEWPORT,
		};

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.dynamicStateCount = (uint32_t)dynamicStates.size();
		dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
		inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyCreateInfo.topology = PrimitiveTopologyTypeToVulkan(m_Properties.PrimitiveTopology);
		inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = (float)windowProperties.Width;
		viewport.height = (float)windowProperties.Height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { windowProperties.Width, windowProperties.Height };

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.pViewports = &viewport;
		viewportStateCreateInfo.scissorCount = 1;
		viewportStateCreateInfo.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
		rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationStateCreateInfo.lineWidth = m_Properties.LineWidth;
		rasterizationStateCreateInfo.polygonMode = PolygonFillTypeToVulkan(m_Properties.PolygonFill);

		rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;

		rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
		rasterizationStateCreateInfo.depthBiasSlopeFactor = 0;
		rasterizationStateCreateInfo.depthBiasClamp = 0;
		rasterizationStateCreateInfo.depthBiasConstantFactor = 0;

		VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = {};
		multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
		multisamplingCreateInfo.minSampleShading = 1.0f;
		multisamplingCreateInfo.pSampleMask = nullptr;
		multisamplingCreateInfo.alphaToCoverageEnable = VK_FALSE;
		multisamplingCreateInfo.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
		colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateCreateInfo.attachmentCount = 1;
		colorBlendStateCreateInfo.pAttachments = &colorBlendAttachment;
		colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
		colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;

		for (auto& iterator : shader->GetShaderModules())
		{
			auto& domain = iterator.first;
			auto& shaderModule = iterator.second;

			VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfo.stage = ShaderDomainToShaderStageFlag(domain);
			shaderStageCreateInfo.module = shaderModule;
			shaderStageCreateInfo.pName = "main";

			shaderStageCreateInfos.push_back(shaderStageCreateInfo);
		}

		std::vector<VkPushConstantRange> pushConstantRanges = shader->GetPushConstantRanges();

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutCreateInfo.setLayoutCount = 0;
		layoutCreateInfo.pSetLayouts = nullptr;
		layoutCreateInfo.pushConstantRangeCount = (uint32_t)pushConstantRanges.size();
		layoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();


		if (vkCreatePipelineLayout(device, &layoutCreateInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Failed to create Vulkan pipeline layout");
			return;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = shader->GetVertexInputStateCreateInfo(ShaderDomain::Vertex);

		VkAttachmentDescription colorAttachmentDescription = {};
		colorAttachmentDescription.format = swapchain->GetFormat();
		colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
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

		pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
		pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
		pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
		pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
		pipelineCreateInfo.pDepthStencilState = nullptr;
		pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
		pipelineCreateInfo.layout = m_PipelineLayout;
		pipelineCreateInfo.stageCount = (uint32_t)shaderStageCreateInfos.size();
		pipelineCreateInfo.pStages = shaderStageCreateInfos.data();
		pipelineCreateInfo.renderPass = m_RenderPass;

		if (vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineCreateInfo, nullptr, &m_Pipeline) != VK_SUCCESS)
		{
			LOG_CORE_ERROR("Failed to create Vulkan pipeline");
			return;
		}

	}

}
