#pragma once

#include "Renderer/Renderer.h"
#include "VulkanContext.h"
#include "VulkanSwapchain.h"


namespace Nutz
{


    class VulkanRenderer : public RendererAPI
    {
    public:
        VulkanRenderer();

        virtual void Shutdown() override;

        virtual void BeginScene() override;
        virtual void EndScene() override;

        virtual void Submit() override;

        virtual void Pump() override;
        virtual void SubmitToRenderThread(std::function<void()> fn);

    private:
        virtual void Initialize() override;

    };


}
