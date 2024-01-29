#include "nutz_pch.h"
#include "RendererContext.h"

#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanContext.h"


namespace Nutz
{

	Ref<RendererContext> RendererContext::Create(void* windowHandle)
	{
		switch (Renderer::API())
		{
			case RendererAPIType::Vulkan: return CreateRef<VulkanContext>(windowHandle);
			case RendererAPIType::OpenGL: return nullptr;
		}

		return nullptr;
	}


}
