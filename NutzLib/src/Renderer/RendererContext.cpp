#include "nutz_pch.h"
#include "RendererContext.h"

#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanContext.h"


namespace Nutz
{

	Ref<RendererContext> RendererContext::Create()
	{
		switch (Renderer::API())
		{
		case RendererAPIType::Vulkan: return CreateRef<VulkanContext>();
		case RendererAPIType::OpenGL: return nullptr;
		}

		return nullptr;
	}


}
