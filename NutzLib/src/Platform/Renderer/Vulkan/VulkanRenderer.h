#pragma once

#include "Renderer/Renderer.h"
#include "VulkanContext.h"


namespace Nutz
{


    class VulkanRenderer : public RendererAPI
    {
    public:
        VulkanRenderer();

        virtual void BeginScene() override;
        virtual void EndScene() override;

    private:
        virtual void Initialize() override;

    };


}
