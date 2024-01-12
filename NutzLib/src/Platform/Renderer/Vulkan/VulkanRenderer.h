#pragma once

#include "Renderer/Renderer.h"
#include "VulkanContext.h"


namespace Nutz
{


    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer(Ref<Window> window);


    private:
        virtual void Initialize() override;


    private:
        Ref<VulkanContext> m_Context = nullptr;

    };


}
