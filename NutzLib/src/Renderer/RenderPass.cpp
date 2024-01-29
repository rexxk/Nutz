#include "nutz_pch.h"
#include "RenderPass.h"


#include "Renderer/Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanRenderPass.h"


namespace Nutz
{


	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& renderPassSpecification)
	{
		switch (Renderer::API())
		{
			case RendererAPIType::Vulkan: return CreateRef<VulkanRenderPass>(renderPassSpecification);
			case RendererAPIType::OpenGL: return nullptr;
		}

		return nullptr;
	}



}
