#pragma once


#include "Renderer/Framebuffer.h"



namespace Nutz
{


	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(const FramebufferSpecification& framebufferSpecification);

		virtual void Shutdown() override;

	private:
		FramebufferSpecification m_Specification;
	};


}
