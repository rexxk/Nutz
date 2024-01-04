#include "nutz_pch.h"
#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanRenderer.h"


namespace Nutz
{


    void Renderer::Create(RendererAPI api, void* windowHandle)
    {
        s_API = api;

        switch (s_API)
        {
            case RendererAPI::Vulkan: s_Renderer = CreateRef<VulkanRenderer>(windowHandle); break;
        }
    }

    Renderer::Renderer(void* windowHandle)
        : m_Handle(windowHandle)
    {

    }


}
