#include "nutz_pch.h"
#include "Mouse.h"

#include "Core/Message/Messages.h"


namespace Nutz
{


    Ref<Mouse> Mouse::Create()
    {
        return CreateRef<Mouse>();
    }



    Mouse::Mouse()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = this;
        }
        else
        {
            LOG_CORE_WARN("Mouse instance already exists");
            return;
        }

        m_ButtonState.resize(10);

//        std::memset(s_Instance->m_ButtonState.data(), false, m_ButtonState.size());

        MessageQueue::Subscribe(MessageType::MouseButtonPressed, [&](Ref<Message> msg)
        {
            Ref<MouseButtonPressedMessage> message = std::dynamic_pointer_cast<MouseButtonPressedMessage>(msg);
            s_Instance->m_ButtonState[message->Button()] = true;

            return false;
        });

        MessageQueue::Subscribe(MessageType::MouseButtonReleased, [&](Ref<Message> msg)
        {
            Ref<MouseButtonReleasedMessage> message = std::dynamic_pointer_cast<MouseButtonReleasedMessage>(msg);
            s_Instance->m_ButtonState[message->Button()] = false;
            
            return false;
        });
    }

    bool Mouse::IsMouseButtonPressed(uint32_t button)
    {
        return s_Instance->m_ButtonState[button];
    }

    bool Mouse::IsMouseButtonReleased(uint32_t button)
    {
        return !s_Instance->m_ButtonState[button];
    }
}
