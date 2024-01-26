#include "nutz_pch.h"
#include "Pipeline.h"

#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanPipeline.h"


namespace Nutz
{


	Ref<Pipeline> Pipeline::Create(const PipelineProperties& pipelineProperties)
	{

		switch (Renderer::API())
		{
		case RendererAPIType::Vulkan: return CreateRef<VulkanPipeline>(pipelineProperties);
		}

		return nullptr;
	}


}
