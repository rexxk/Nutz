#include "nutz_pch.h"
#include "Core/Application.h"
#include "Core/Timer.h"
#include "Core/Timestep.h"

#include "Message/Messages.h"



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
				LOG_CORE_TRACE("Window resized : {},{}", message->Width(), message->Height());

				return false;
			});

	}


	void Application::Run()
	{

		m_Running = true;

		Timer fpsTimer;
		Timestep frameTime;

		uint32_t fps = 0;

		while (m_Running)
		{
			m_MainWindow->HandleEvents();
			MessageQueue::Process();

			for (auto& layer : *m_LayerStack)
			{
				layer->OnUpdate(frameTime);
			}

			fps++;

			frameTime.Reset();

			if (fpsTimer.ElapsedTime() > 1.0)
			{
				LOG_CORE_TRACE("FPS: {}", fps);
				fpsTimer.Reset();
				fps = 0;
			}
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

