#pragma once

#include "RendererAPI.h"

#include "Core/Window.h"


namespace Nutz
{


    class Renderer
    {
    public:
        static void Create(RendererAPI api, Ref<Window> window);

        Renderer(Ref<Window> window);

        static RendererAPI API() { return s_API; }
        static Ref<Renderer> Get() { return s_Renderer; }

    private:
        inline static RendererAPI s_API = RendererAPI::None;
        inline static Ref<Renderer> s_Renderer = nullptr;

        virtual void Initialize();


    protected:
        Ref<Window> m_Window = nullptr;

    };


}
