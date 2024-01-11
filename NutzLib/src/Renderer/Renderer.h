#pragma once

#include "RendererAPI.h"



namespace Nutz
{


    class Renderer
    {
    public:
        static void Create(RendererAPI api, void* windowHandle, uint32_t width, uint32_t height);

        Renderer(void* windowHandle);

        static RendererAPI API() { return s_API; }
        static Ref<Renderer> Get() { return s_Renderer; }

    private:
        inline static RendererAPI s_API = RendererAPI::None;
        inline static Ref<Renderer> s_Renderer = nullptr;

        virtual void Initialize(uint32_t width, uint32_t height);


    protected:
        void* m_Handle = nullptr;

    };


}
