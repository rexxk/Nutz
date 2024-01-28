#pragma once



namespace Nutz
{


	class RendererContext
	{
	public:
		static Ref<RendererContext> Create();

		virtual ~RendererContext() {}
		virtual void Shutdown() = 0;
	};


}
