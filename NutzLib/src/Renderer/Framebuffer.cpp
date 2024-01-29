#include "nutz_pch.h"
#include "Framebuffer.h"

#include "Renderer.h"


namespace Nutz
{


	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& framebufferSpecification)
	{
		switch (Renderer::API())
		{
			case RendererAPIType::Vulkan: return CreateRef<VulkanFramebuffer>(framebufferSpecification);
		}

		return nullptr;
	}



}
