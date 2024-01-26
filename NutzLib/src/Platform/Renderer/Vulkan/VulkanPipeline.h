#pragma once

#include "Renderer/Pipeline.h"

#include "VulkanDevice.h"

#include <vulkan/vulkan.h>



namespace Nutz
{


	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineProperties& pipelineProperties);

		void Shutdown();

	private:
		void CreatePipeline();

	private:

		VkPipeline m_Pipeline = nullptr;
		VkPipelineLayout m_PipelineLayout = nullptr;


		PipelineProperties m_Properties;

	};


}
