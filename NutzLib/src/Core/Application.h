#pragma once

#include "LayerStack.h"

#include "Window.h"

#include "Input/Keyboard.h"
#include "Input/Mouse.h"

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
		Ref<Window> m_MainWindow = nullptr;

		Ref<Keyboard> m_Keyboard = nullptr;
		Ref<Mouse> m_Mouse = nullptr;

		bool m_Running = false;

	};



}
