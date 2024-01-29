#include "nutz_pch.h"
#include "Renderer.h"

#include "Renderer/Shader.h"

#include "Platform/Renderer/Vulkan/VulkanRenderer.h"


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


        Initialize();

    }


    RendererAPI::RendererAPI()
    {

    }


    void Renderer::Initialize()
    {
//        Nutz::PipelineProperties pipelineProps;
//        pipelineProps.Shader = Nutz::ShaderLibrary::Get("basic");
//        Nutz::Pipeline::Create(pipelineProps);




        if (s_Renderer != nullptr)
        {
            s_Renderer->Initialize();
        }

        Nutz::ShaderLibrary::Load("assets/shaders/basic.shader");

    }

}
