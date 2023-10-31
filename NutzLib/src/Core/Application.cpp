#include "nutz_pch.h"
#include "Core/Application.h"



namespace Nutz
{


	Application::Application()
	{
	
		m_LayerStack = CreateScope<LayerStack>();

		WindowProperties windowProps;
		windowProps.Width = 1280;
		windowProps.Height = 720;
		windowProps.Title = "Nutz Engine";
		windowProps.Mode = WindowMode::Windowed;
		windowProps.VSync = false;

		m_MainWindow = CreateScope<Window>(windowProps);



	}


	void Application::Run()
	{


		while (true)
		{
			m_MainWindow->HandleEvents();


		}


	}


	void Application::Shutdown()
	{

	}


	void Application::AttachLayer(Ref<Layer>& layer)
	{
		m_LayerStack->AttachLayer(layer);
	}

	void Application::DetachLayer(Ref<Layer>& layer)
	{
		m_LayerStack->DetachLayer(layer);
	}

}

