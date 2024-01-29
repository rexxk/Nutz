#pragma once

#include "Renderer/RenderPass.h"


namespace Nutz
{


	struct FramebufferSpecification
	{
		Ref<RenderPass> RenderPass = nullptr;

		uint32_t Width = 0;
		uint32_t Height = 0;
	};


	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpecification& framebufferSpecification);

		virtual void Shutdown() = 0;

	};


}
