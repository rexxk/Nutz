#include "EditorApp.h"



EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{

    Nutz::WindowProperties windowProps;

    windowProps.Width = 1280;
    windowProps.Height = 720;
    windowProps.Title = "Nut Editor v1.0";
    windowProps.VSync = false;
    windowProps.Mode = Nutz::WindowMode::Maximized;

    m_Window = Nutz::Window::Create(windowProps);

    // CreateRenderer(API..., windowHandle)....

    Nutz::AssetManager::Clear();
}

void EditorLayer::OnUpdate(Nutz::Timestep ts)
{
    m_Window->HandleEvents();
}
