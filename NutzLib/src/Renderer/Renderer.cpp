#include "nutz_pch.h"
#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanRenderer.h"


namespace Nutz
{


    void Renderer::Create(RendererAPIType apiType, Ref<Window> window)
    {
        s_API = apiType;

        switch (s_API)
        {
            case RendererAPIType::Vulkan: s_Renderer = CreateRef<VulkanRenderer>(window); break;
            case RendererAPIType::OpenGL: s_Renderer = nullptr; break;
        }


        Initialize();

    }


    RendererAPI::RendererAPI(Ref<Window> window)
        : m_Window(window)
    {

    }


    void Renderer::Initialize()
    {



        if (s_Renderer != nullptr)
        {
            s_Renderer->Initialize();
        }

    }

}
