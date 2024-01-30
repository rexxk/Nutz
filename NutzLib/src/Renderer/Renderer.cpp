#include "nutz_pch.h"
#include "Renderer.h"

#include "Renderer/Shader.h"

#include "Platform/Renderer/Vulkan/VulkanRenderer.h"

#include "Core/Application.h"


namespace Nutz
{


    void Renderer::Create(RendererAPIType apiType)
    {
        s_API = apiType;

        switch (s_API)
        {
            case RendererAPIType::Vulkan: s_Renderer = CreateRef<VulkanRenderer>(); break;
            case RendererAPIType::OpenGL: s_Renderer = nullptr; break;
        }

    }


    RendererAPI::RendererAPI()
    {

    }


    void Renderer::Initialize()
    {
        if (s_Renderer != nullptr)
        {
            s_Renderer->Initialize();
        }

    }

    void Renderer::Shutdown()
    {
        s_Renderer->Shutdown();
    }

    Ref<RendererContext> Renderer::GetContext() { return Application::Get().GetWindow()->GetRendererContext(); }

    void Renderer::BeginScene()
    {
        s_Renderer->BeginScene();
    }

    void Renderer::EndScene()
    {
        s_Renderer->EndScene();
    }

}
