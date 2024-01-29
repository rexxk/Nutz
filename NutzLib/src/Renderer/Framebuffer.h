#pragma once



namespace Nutz
{


	struct FramebufferSpecification
	{
		
	};


	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpecification& framebufferSpecification);

		virtual void Shutdown() = 0;

	};


}
