#include "nutz_pch.h"
#include "Core/Application.h"
#include "Core/Timer.h"
#include "Core/Timestep.h"

#include "Message/Messages.h"

#include "Utils/Statistics.h"

#include "Core/Input/Keydefs.h"


namespace Nutz
{


	Application::Application()
	{
		if (s_Instance)
		{
			LOG_CORE_ERROR("Application instance already exists");
			return;
		}

		s_Instance = this;
	
		m_LayerStack = CreateScope<LayerStack>();

//		WindowProperties windowProps;
//		windowProps.Width = 1280;
//		windowProps.Height = 720;
//		windowProps.Title = "Nutz Engine";
//		windowProps.Mode = WindowMode::Windowed;
//		windowProps.VSync = false;
//
//		m_MainWindow = Window::Create(windowProps);

		m_Keyboard = Keyboard::Create();
		m_Mouse = Mouse::Create();
		

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

		Statistics& stats = Statistics::Get();

		while (m_Running)
		{
//			m_MainWindow->HandleEvents();
			MessageQueue::Process();

			for (auto& layer : *m_LayerStack)
			{
				layer->OnUpdate(frameTime);
			}

			stats.FPS++;

			frameTime.Reset();

			if (fpsTimer.ElapsedTime() > 1.0)
			{
				LOG_CORE_TRACE("FPS: {}", stats.FPS);
				fpsTimer.Reset();
				stats.FPS = 0;
			}

			if (Keyboard::IsKeyPressed(NUTZ_KEY_ESCAPE))
				m_Running = false;
		}


	}


	void Application::Shutdown()
	{
		m_Window->ShutdownRendererContext();
	}


	void Application::AttachLayer(Ref<Layer>& layer)
	{
		m_LayerStack->AttachLayer(layer);
	}

	void Application::DetachLayer(Ref<Layer>& layer)
	{
		m_LayerStack->DetachLayer(layer);
	}

	void Application::CreateWindow(const WindowProperties& windowProperties)
	{
		if (s_Instance)
			s_Instance->m_Window = Window::Create(windowProperties);
	}


}

