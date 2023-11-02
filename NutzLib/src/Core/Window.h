#pragma once



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
		uint32_t Width;
		uint32_t Height;
		std::string Title;

		bool VSync;

		WindowMode Mode;
	};


	class Window
	{
	public:
		Window(const WindowProperties& props);
		~Window();

		std::pair<uint32_t, uint32_t> Size() { return std::make_pair(m_Properties.Width, m_Properties.Height); }

		void* GetHandle() { return m_Handle; }

		void HandleEvents();

	private:
		void CreateWindow();

	private:
		void* m_Handle;

		WindowProperties m_Properties;
	};


}
