#include "nutz_pch.h"
#include "Window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef CreateWindow

#include <iostream>

#include "Message/Message.h"
#include "Message/MessageQueue.h"


namespace Nutz
{



	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	Window::Window(const WindowProperties& props)
		: m_Properties(props)
	{
		CreateWindow();
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
			std::cout << "Failed to register window class.\n";
			return;
		}

		m_Handle = CreateWindowW(L"NutzEngine", L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Properties.Width, m_Properties.Height, NULL, NULL, GetModuleHandle(NULL), NULL);

		ShowWindow((HWND)m_Handle, SW_SHOW);
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
