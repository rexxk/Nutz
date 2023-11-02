#pragma once

#include "Message.h"

#include <cstdint>


namespace Nutz
{

	class WindowClosedMessage : public Message
	{
	public:
		WindowClosedMessage(void* windowHandle)
		{
			Type = MessageType::WindowClosed;
		}

		void* Handle() const { return m_Handle; }

	private:
		void* m_Handle = nullptr;
	};


	class WindowResizedMessage : public Message
	{
	public:
		WindowResizedMessage(void* windowHandle, uint32_t width, uint32_t height)
			: m_Handle(windowHandle), m_Width(width), m_Height(height)
		{
			Type = MessageType::WindowResized;
		}

		void* Handle() const { return m_Handle; }

		uint32_t Width() const { return m_Width; }
		uint32_t Height() const { return m_Height; }

	private:
		void* m_Handle;
		uint32_t m_Width;
		uint32_t m_Height;

	};

}
