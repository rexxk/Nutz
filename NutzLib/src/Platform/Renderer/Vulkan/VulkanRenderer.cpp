#include "nutz_pch.h"
#include "VulkanRenderer.h"

#include "VulkanContext.h"
#include "VulkanShader.h"

#include "Core/Application.h"


namespace Nutz
{


    VulkanRenderer::VulkanRenderer()
    {
        LOG_CORE_TRACE("Creating Vulkan renderer");

    }



    void VulkanRenderer::Initialize()
    {
        LOG_CORE_TRACE("Initializing Vulkan context");

        Nutz::ShaderLibrary::Load("assets/shaders/basic.shader");

    }

    void VulkanRenderer::Shutdown()
    {

    }

    void VulkanRenderer::BeginScene()
    {

    }

    void VulkanRenderer::EndScene()
    {

    }

}
