#pragma once



namespace Nutz
{


	class RendererContext
	{
	public:
		static Ref<RendererContext> Create(void* windowHandle);

		virtual ~RendererContext() {}
		virtual void Shutdown() = 0;
	};


}
