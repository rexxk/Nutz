#pragma once

#ifdef _WIN32
#undef CreateWindow
#endif

#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererContext.h"



namespace Nutz
{


	enum class WindowMode
	{
		Windowed,
		Maximized,
		Fullscreen,
	};


	struct WindowProperties
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;
		std::string Title = "Default Title";

		bool VSync = false;

		WindowMode Mode = WindowMode::Windowed;
	};

	class VulkanSwapchain;

	class Window
	{
	public:
		static Ref<Window> Create(const WindowProperties& props);

		Window(const WindowProperties& props);
		virtual ~Window();

		virtual void SetupRenderer() = 0;
		
		virtual void Present() {}

		std::pair<uint32_t, uint32_t> Size() { return std::make_pair(m_Properties.Width, m_Properties.Height); }

		void* GetHandle() { return m_Handle; }
		WindowProperties& GetProperties() { return m_Properties; }
		Ref<RendererContext> GetRendererContext() { return m_RendererContext; }

		virtual void HandleEvents() {}

	private:
		virtual void CreateWindow() {}

	protected:
		void* m_Handle;

		WindowProperties m_Properties;

		Ref<RendererContext> m_RendererContext = nullptr;
		Ref<VulkanSwapchain> m_Swapchain = nullptr;

	};


}
