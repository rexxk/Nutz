#pragma once

#include "RendererAPI.h"



namespace Nutz
{


    class Renderer
    {
    public:
        static void Create(RendererAPI api, void* windowHandle);

        Renderer(void* windowHandle);

        static RendererAPI API() { return s_API; }
        static Ref<Renderer> Get() { return s_Renderer; }

    private:
        inline static RendererAPI s_API = RendererAPI::None;
        inline static Ref<Renderer> s_Renderer = nullptr;

        virtual void Initialize();


    protected:
        void* m_Handle = nullptr;

    };


}
