#include "nutz_pch.h"
#include "VulkanRenderer.h"

#include "VulkanContext.h"
#include "VulkanShader.h"
#include "VulkanSwapchain.h"

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
        Ref<VulkanSwapchain> swapchain = VulkanSwapchain::Get();

        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        VkCommandBuffer currentCommandBuffer = swapchain->GetCurrentCommandBuffer();

        vkBeginCommandBuffer(currentCommandBuffer, &commandBufferBeginInfo);


        VkRenderPassBeginInfo renderpassBeginInfo = {};
        renderpassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpassBeginInfo.renderPass = swapchain->GetRenderPass();
        renderpassBeginInfo.framebuffer = swapchain->GetCurrentFramebuffer();
        renderpassBeginInfo.renderArea.offset = { 0, 0 };
        renderpassBeginInfo.renderArea.extent = swapchain->GetCurrentExtent();

        std::array<VkClearValue, 1> clearValues = {};
        clearValues[0].color = { { 0.2f, 0.3f, 0.45f, 1.0f } };

        renderpassBeginInfo.clearValueCount = (uint32_t)clearValues.size();
        renderpassBeginInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(currentCommandBuffer, &renderpassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    }

    void VulkanRenderer::EndScene()
    {
        Ref<VulkanSwapchain> swapchain = VulkanSwapchain::Get();

        VkCommandBuffer currentCommandBuffer = swapchain->GetCurrentCommandBuffer();

        vkCmdEndRenderPass(currentCommandBuffer);

        vkEndCommandBuffer(currentCommandBuffer);

    }

}
