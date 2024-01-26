#pragma once

#include "Renderer/Renderer.h"
#include "VulkanContext.h"


namespace Nutz
{


    class VulkanRenderer : public RendererAPI
    {
    public:
        VulkanRenderer();


    private:
        virtual void Initialize() override;

    };


}
