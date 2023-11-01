#include "nutz_pch.h"
#include "Core/Application.h"


#include "Message/Message.h"
#include "Message/MessageQueue.h"

#include <iostream>


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



		MessageQueue::Subscribe(MessageType::WindowClosed, [&](Ref<Message> msg)
			{
				m_Running = false;
				return false;
			});

		MessageQueue::Subscribe(MessageType::WindowResized, [&](Ref<Message> msg)
			{
				Ref<WindowResizedMessage> message = std::dynamic_pointer_cast<WindowResizedMessage>(msg);
				std::cout << "Window resized: " << message->Width() << ", " << message->Height() << "\n";

				return false;
			});

	}


	void Application::Run()
	{

		m_Running = true;

		while (m_Running)
		{
			m_MainWindow->HandleEvents();
			MessageQueue::Process();

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

