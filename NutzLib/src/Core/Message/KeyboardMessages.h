#pragma once

#include "Message.h"

#include <cstdint>


namespace Nutz
{

    class KeyPressedMessage : public Message
    {
    public:
        KeyPressedMessage(int key, int scancode)
            : m_Scancode((uint32_t)scancode), m_Key((uint32_t)key)
        {
            Type = MessageType::KeyPressed;
        }

        uint32_t Key() const { return m_Key; }
        uint32_t Scancode() const { return m_Scancode; }

    private:
        uint32_t m_Key = 0;
        uint32_t m_Scancode = 0;
    };

    class KeyReleasedMessage : public Message
    {
    public:
        KeyReleasedMessage(int key, int scancode)
            : m_Scancode((uint32_t)scancode), m_Key((uint32_t)key)
        {
            Type = MessageType::KeyReleased;
        }

        uint32_t Key() const { return m_Key; }
        uint32_t Scancode() const { return m_Scancode; }

    private:
        uint32_t m_Key = 0;
        uint32_t m_Scancode = 0;
    };

}
