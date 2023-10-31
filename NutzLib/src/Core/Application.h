#pragma once

#include "LayerStack.h"



namespace Nutz
{


	class Application
	{
	public:
		Application();

		void Run();

		void Shutdown();


		void AttachLayer(Ref<Layer>& layer);
		void DetachLayer(Ref<Layer>& layer);

	private:
		Scope<LayerStack> m_LayerStack = nullptr;

	};



}
