#pragma once


namespace Nutz
{

    class Keyboard
    {
    public:
        static Ref<Keyboard> Create();

        Keyboard();


        static bool IsKeyPressed(uint32_t scancode);
        static bool IsKeyReleased(uint32_t scancode);

    private:
        inline static Keyboard* s_Instance = nullptr; 

        std::vector<bool> m_KeyState;

    };


}
