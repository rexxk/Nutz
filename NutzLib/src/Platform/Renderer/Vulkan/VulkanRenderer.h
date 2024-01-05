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
        virtual void Initialize() override;


    private:
        Ref<VulkanContext> m_Context = nullptr;

    };


}
