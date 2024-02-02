#pragma once

#include "RendererAPI.h"

#include "RenderCommandQueue.h"

#include "Core/RenderThread.h"


namespace Nutz
{


    class RendererContext;

    class Renderer
    {
    public:
        static void Create(RendererAPIType apiType);

        static RendererAPIType API() { return s_API; }
        static Ref<RendererAPI> Get() { return s_Renderer; }


        template<typename FuncT>
        static void Submit(FuncT&& func)
        {
            auto renderCommand = [](void* ptr)
                {
                    auto pFunc = (FuncT*)ptr;
                    (*pFunc)();

                    pFunc->~FuncT();
                };

            auto storageBuffer = GetRenderCommandQueue().Allocate(renderCommand, sizeof(func));
            new (storageBuffer) FuncT(std::forward<FuncT>(func));
        }


        static void Initialize();
        static void Shutdown();

        static void BeginScene();
        static void EndScene();

        static Ref<RendererContext> GetContext();


        static uint32_t GetRenderCommandQueueSubmissionIndex();
        static uint32_t GetRenderQueueIndex();


        static void RenderThreadFunction(RenderThread* renderThread);
        static void WaitAndRender(RenderThread* renderThread);
        static void SwapQueues();


    private:
        inline static RendererAPIType s_API = RendererAPIType::None;
        inline static Ref<RendererAPI> s_Renderer = nullptr;

        static RenderCommandQueue& GetRenderCommandQueue();
    };


}
