#include "nutz_pch.h"
#include "WindowsWindow.h"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef CreateWindow

#include "Message/Messages.h"


namespace Nutz
{



	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	Window::Window(const WindowProperties& props)
		: m_Properties(props)
	{
		CreateWindow();
	
		MessageQueue::Subscribe(MessageType::WindowResized, [&](Ref<Message> msg)
			{
				Ref<WindowResizedMessage> message = std::dynamic_pointer_cast<WindowResizedMessage>(msg);
				m_Properties.Width = message->Width();
				m_Properties.Height = message->Height();

				return false;
			});
	}

	Window::~Window()
	{

	}


	void Window::CreateWindow()
	{
		WNDCLASSEX wcex = {0};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.lpszClassName = L"NutzEngine";
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc = WndProc;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

		if (!RegisterClassEx(&wcex))
		{
			LOG_CORE_ERROR("Failed to register window class.");
			return;
		}

		m_Handle = CreateWindowW(L"NutzEngine", L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Properties.Width, m_Properties.Height, NULL, NULL, GetModuleHandle(NULL), NULL);

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		RECT rc;
		GetWindowRect((HWND)m_Handle, &rc);
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		SetWindowPos((HWND)m_Handle, NULL, (screenWidth - m_Properties.Width) / 2, (screenHeight - m_Properties.Height) / 2, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);

		if (m_Properties.Mode == WindowMode::Windowed)
		{
			ShowWindow((HWND)m_Handle, SW_SHOW);
		}
		else if (m_Properties.Mode == WindowMode::Maximized)
		{
			ShowWindow((HWND)m_Handle, SW_SHOWMAXIMIZED);
		}

	}

	void Window::HandleEvents()
	{
		MSG msg;

		while (PeekMessage(&msg, (HWND)m_Handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message)
		{
			case WM_DESTROY:
			{
				Ref<Message> message = CreateRef<WindowClosedMessage>((void*)wnd);
				MessageQueue::Add(message);

				PostQuitMessage(0);
				return 0;
			}

			case WM_SIZE:
			{
				Ref<Message> message = CreateRef<WindowResizedMessage>((void*)wnd, LOWORD(lParam), HIWORD(lParam));
				MessageQueue::Add(message);

				break;
			}
		}


		return DefWindowProc(wnd, message, wParam, lParam);
	}

}

#endif
