#include "nutz_pch.h"
#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanRenderer.h"


namespace Nutz
{


    void Renderer::Create(RendererAPI api, void* windowHandle, uint32_t width, uint32_t height)
    {
        s_API = api;

        switch (s_API)
        {
            case RendererAPI::Vulkan: s_Renderer = CreateRef<VulkanRenderer>(windowHandle); break;
            case RendererAPI::OpenGL: s_Renderer = nullptr; break;
        }


        if (s_Renderer != nullptr)
        {
            s_Renderer->Initialize(width, height);
        }
    }

    Renderer::Renderer(void* windowHandle)
        : m_Handle(windowHandle)
    {

    }


    void Renderer::Initialize(uint32_t width, uint32_t height)
    {

    }

}
