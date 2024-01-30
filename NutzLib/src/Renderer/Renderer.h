#pragma once

#include "RendererAPI.h"


namespace Nutz
{


    class Renderer
    {
    public:
        static void Create(RendererAPIType apiType);

        static RendererAPIType API() { return s_API; }
        static Ref<RendererAPI> Get() { return s_Renderer; }

        static void Initialize();
        static void Shutdown();

    private:
        inline static RendererAPIType s_API = RendererAPIType::None;
        inline static Ref<RendererAPI> s_Renderer = nullptr;

    };


}
