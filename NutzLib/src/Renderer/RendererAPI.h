#pragma once

#include "Core/Window.h"


namespace Nutz
{


    enum class RendererAPIType
    {
        None,
        OpenGL,
        Vulkan,
        DirectX11,
        DirectX12,
    };


    class RendererAPI
    {
    public:
        RendererAPI();

        virtual void Initialize() = 0;

        virtual void BeginScene() = 0;
        virtual void EndScene() = 0;

    };

}
