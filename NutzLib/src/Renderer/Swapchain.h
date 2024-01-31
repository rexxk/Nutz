#pragma once




namespace Nutz
{


	class Swapchain
	{
	public:

		virtual void Shutdown() = 0;

		virtual void BeginFrame() = 0;
		virtual void Present() = 0;

		static Ref<Swapchain> Get();

	};


}
