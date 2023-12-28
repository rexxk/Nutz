#pragma once

#include "Message.h"

#include <cstdint>


namespace Nutz
{

    class KeyPressedMessage : public Message
    {
    public:
        KeyPressedMessage(int scancode)
            : m_Scancode((uint32_t)scancode)
        {
            Type = MessageType::KeyPressed;
        }

        uint32_t Scancode() const { return m_Scancode; }

    private:
        uint32_t m_Scancode = 0;
    };

    class KeyReleasedMessage : public Message
    {
    public:
        KeyReleasedMessage(int scancode)
            : m_Scancode(scancode)
        {
            Type = MessageType::KeyReleased;
        }

        uint32_t Scancode() const { return m_Scancode; }

    private:
        uint32_t m_Scancode = 0;
    };

}
