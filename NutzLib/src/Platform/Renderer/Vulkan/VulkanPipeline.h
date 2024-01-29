#pragma once

#include "Renderer/Pipeline.h"
#include "Renderer/RenderPass.h"

#include "VulkanDevice.h"

#include <vulkan/vulkan.h>



namespace Nutz
{


	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineProperties& pipelineProperties);

		virtual void Shutdown() override;

		virtual void Bind() override;

	private:
		void CreatePipeline();

	private:

		VkPipeline m_Pipeline = nullptr;
		VkPipelineLayout m_PipelineLayout = nullptr;

		Ref<RenderPass> m_RenderPass = nullptr;

		PipelineProperties m_Properties;

	};


}
