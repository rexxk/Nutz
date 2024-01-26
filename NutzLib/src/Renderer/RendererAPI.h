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
        RendererAPI(Ref<Window> window);

        virtual void Initialize() = 0;


    protected:
        Ref<Window> m_Window = nullptr;

    };

}
