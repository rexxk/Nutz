#pragma once



namespace Nutz
{

    class Mouse
    {
    public:
        static Ref<Mouse> Create();

        Mouse();

        static bool IsMouseButtonPressed(uint32_t button);
        static bool IsMouseButtonReleased(uint32_t button);


    private:
        inline static Mouse* s_Instance = nullptr;

        std::vector<bool> m_ButtonState;
    };

}
