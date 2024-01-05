#include "nutz_pch.h"
#include "Keyboard.h"

#include "Keydefs.h"

#include "Core/Message/Messages.h"

namespace Nutz
{


    Ref<Keyboard> Keyboard::Create()
    {
        return CreateRef<Keyboard>();
    }


    Keyboard::Keyboard()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = this;
        }
        else
        {
            LOG_CORE_WARN("Keyboard instance already exists");
            return;
        }

        m_KeyState.resize(NUTZ_MAX_SCANCODE);

        for (auto it = m_KeyState.begin(); it != m_KeyState.end(); it++)
        {
            *it = false;
        }

        MessageQueue::Subscribe(MessageType::KeyPressed, [&](Ref<Message> msg)
        {
           
            Ref<KeyPressedMessage> message = std::dynamic_pointer_cast<KeyPressedMessage>(msg);
            s_Instance->m_KeyState[message->Key()] = true;

//            LOG_CORE_TRACE("Key pressed: {}", message->Key());

            return false;
        });

        MessageQueue::Subscribe(MessageType::KeyReleased, [&](Ref<Message> msg)
        {
            Ref<KeyReleasedMessage> message = std::dynamic_pointer_cast<KeyReleasedMessage>(msg);
            s_Instance->m_KeyState[message->Key()] = false;

//            LOG_CORE_TRACE("Key released");

            return false;
        });
    }

    bool Keyboard::IsKeyPressed(uint32_t scancode)
    {
        return s_Instance->m_KeyState[scancode];
    }

    bool Keyboard::IsKeyReleased(uint32_t scancode)
    {
        return !s_Instance->m_KeyState[scancode];
    }

}
