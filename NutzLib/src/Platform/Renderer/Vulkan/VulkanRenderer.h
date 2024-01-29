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


        Ref<VulkanSwapchain> GetSwapchain() { return m_Swapchain; }

    private:
        virtual void Initialize() override;

        Ref<VulkanSwapchain> m_Swapchain = nullptr;

    };


}
