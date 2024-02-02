#include "nutz_pch.h"
#include "Core/Application.h"
#include "Core/Timer.h"
#include "Core/Timestep.h"
#include "Core/RenderThread.h"

#include "Message/Messages.h"

#include "Utils/Statistics.h"

#include "Core/Input/Keydefs.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Platform/Renderer/Vulkan/VulkanSwapchain.h"

#include <GLFW/glfw3.h>

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

				if (message->Width() == 0 || message->Height() == 0)
				{
					m_Window->SetWindowMode(WindowMode::Minimized);
					return false;
				}

				if (glfwGetWindowAttrib((GLFWwindow*)message->Handle(), GLFW_MAXIMIZED))
				{
					m_Window->SetWindowMode(WindowMode::Maximized);
					return false;
				}

				m_Window->SetWindowMode(WindowMode::Windowed);

				return false;
			});

	}

	Application::~Application()
	{

	}


	void Application::Run()
	{

		m_Running = true;

		Timer fpsTimer;
		Timestep frameTime;

		Statistics& stats = Statistics::Get();

		RenderThread renderThread;
		renderThread.Run();

		while (m_Running)
		{
//			m_MainWindow->HandleEvents();
			MessageQueue::Process();


			if (m_Window->GetWindowMode() == WindowMode::Minimized)
			{
				m_Window->HandleEvents();
				continue;
			}

			renderThread.Pump();

			m_Window->GetSwapchain()->BeginFrame();

			Renderer::BeginScene();

			for (auto& layer : *m_LayerStack)
			{
				layer->OnUpdate(frameTime);
			}


			Renderer::EndScene();

			m_Window->Present();


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

		renderThread.Terminate();

	}


	void Application::Shutdown()
	{
		for (auto& layer : *m_LayerStack)
		{
			layer->OnDetach();
		}


		ShaderLibrary::Shutdown();
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
		Renderer::Create(RendererAPIType::Vulkan);

		if (s_Instance)
		{
			s_Instance->m_Window = Window::Create(windowProperties);

			s_Instance->m_Window->SetupRenderer();
		}
	}


}

