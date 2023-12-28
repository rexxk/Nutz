#pragma once

#include "Message.h"

#include <cstdint>


namespace Nutz
{

    class MouseButtonPressedMessage : public Message
    {
    public:
        MouseButtonPressedMessage(int button)
            : m_Button((uint32_t)button)
        {
            Type = MessageType::MouseButtonPressed;
        }

        uint32_t Button() const { return m_Button; }

    private:
        uint32_t m_Button;
    };

    class MouseButtonReleasedMessage : public Message
    {
    public:
        MouseButtonReleasedMessage(int button)
            : m_Button((uint32_t)button)
        {
            Type = MessageType::MouseButtonReleased;
        }

        uint32_t Button() const { return m_Button; }

    private:
        uint32_t m_Button;
    };

    

}
