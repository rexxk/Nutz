#include "nutz_pch.h"
#include "VulkanRenderer.h"

#include "VulkanContext.h"

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

        Application::Get().GetWindow()->CreateRendererContext();

        m_Swapchain = VulkanSwapchain::Create(std::dynamic_pointer_cast<VulkanContext>(Application::Get().GetWindow()->GetRendererContext())->GetContextData());

    }

    void VulkanRenderer::Shutdown()
    {
        if (m_Swapchain != nullptr)
        {
            m_Swapchain->Shutdown();
        }
    }

    void VulkanRenderer::BeginScene()
    {

    }

    void VulkanRenderer::EndScene()
    {

    }

}
