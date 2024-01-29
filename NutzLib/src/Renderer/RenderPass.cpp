#include "nutz_pch.h"
#include "RenderPass.h"


#include "Renderer/Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanRenderPass.h"


namespace Nutz
{


	Ref<RenderPass> RenderPass::Create()
	{
		switch (Renderer::API())
		{
			case RendererAPIType::Vulkan: return CreateRef<VulkanRenderPass>();
			case RendererAPIType::OpenGL: return nullptr;
		}

		return nullptr;
	}



}
