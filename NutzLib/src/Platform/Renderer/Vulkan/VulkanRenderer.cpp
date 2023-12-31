#include "nutz_pch.h"
#include "VulkanRenderer.h"




namespace Nutz
{


    VulkanRenderer::VulkanRenderer(void* windowHandle)
        : Renderer(windowHandle)
    {
        LOG_CORE_TRACE("Creating Vulkan renderer");

    }



    void VulkanRenderer::Initialize()
    {
        LOG_CORE_TRACE("Initializing Vulkan context");

        m_Context = VulkanContext::Create(m_Handle);
    }


}
