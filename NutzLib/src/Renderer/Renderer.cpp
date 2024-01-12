#include "nutz_pch.h"
#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanRenderer.h"


namespace Nutz
{


    void Renderer::Create(RendererAPI api, Ref<Window> window)
    {
        s_API = api;

        switch (s_API)
        {
            case RendererAPI::Vulkan: s_Renderer = CreateRef<VulkanRenderer>(window); break;
            case RendererAPI::OpenGL: s_Renderer = nullptr; break;
        }


        if (s_Renderer != nullptr)
        {
            s_Renderer->Initialize();
        }
    }

    Renderer::Renderer(Ref<Window> window)
        : m_Window(window)
    {

    }


    void Renderer::Initialize()
    {

    }

}
