#pragma once

#include "Renderer/Renderer.h"
#include "VulkanContext.h"


namespace Nutz
{


    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer(void* windowHandle);


    private:
        virtual void Initialize(uint32_t width, uint32_t height) override;


    private:
        Ref<VulkanContext> m_Context = nullptr;

    };


}
