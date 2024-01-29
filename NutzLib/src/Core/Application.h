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
		virtual ~Application();

		void Run();

		void Shutdown();


		void AttachLayer(Ref<Layer>& layer);
		void DetachLayer(Ref<Layer>& layer);

		static void CreateWindow(const WindowProperties& windowProperties);

		static Application& Get() { return *s_Instance; }
		static Ref<Window> GetWindow() { return s_Instance->m_Window; }

	private:
		inline static Application* s_Instance = nullptr;

		Scope<LayerStack> m_LayerStack = nullptr;
		Ref<Window> m_Window = nullptr;

		Ref<Keyboard> m_Keyboard = nullptr;
		Ref<Mouse> m_Mouse = nullptr;

		bool m_Running = false;

	};



}
