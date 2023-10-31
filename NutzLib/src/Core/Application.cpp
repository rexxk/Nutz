#include "nutz_pch.h"
#include "Core/Application.h"



namespace Nutz
{


	Application::Application()
	{
	
		m_LayerStack = CreateScope<LayerStack>();




	}


	void Application::Run()
	{

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

