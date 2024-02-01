#include "nutz_pch.h"
#include "Renderer.h"

#include "Renderer/Shader.h"

#include "Platform/Renderer/Vulkan/VulkanRenderer.h"

#include "Core/Application.h"


namespace Nutz
{

    constexpr static uint32_t s_RenderCommandQueueCount = 2;
    static RenderCommandQueue* s_RenderCommandQueues[s_RenderCommandQueueCount];
    static std::atomic<uint32_t> s_RenderCommandQueueSubmissionIndex = 0;


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

    RenderCommandQueue& Renderer::GetRenderCommandQueue() { return *s_RenderCommandQueues[s_RenderCommandQueueSubmissionIndex]; }
    
    uint32_t Renderer::GetRenderCommandQueueSubmissionIndex() { return s_RenderCommandQueueSubmissionIndex; }

    uint32_t Renderer::GetRenderQueueIndex() { return (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount; }

    void Renderer::Initialize()
    {

        s_RenderCommandQueues[0] = new RenderCommandQueue();
        s_RenderCommandQueues[1] = new RenderCommandQueue();


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

    void Renderer::RenderThreadFunction(RenderThread* renderThread)
    {
        while (renderThread->IsRunning())
        {
            WaitAndRender(renderThread);
        }
    }

    void Renderer::WaitAndRender(RenderThread* renderThread)
    {
        renderThread->WaitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

        s_RenderCommandQueues[GetRenderQueueIndex()]->Execute();

        renderThread->Set(RenderThread::State::Idle);
    }

    void Renderer::SwapQueues()
    {
        s_RenderCommandQueueSubmissionIndex = (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
    }

    void Renderer::Submit()
    {
        s_Renderer->Submit();
    }
}
