#include "EditorApp.h"



EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{

    Nutz::WindowProperties windowProps;

    windowProps.Width = 1280;
    windowProps.Height = 720;
    windowProps.Title = "Nut Editor v1.0";
    windowProps.VSync = false;
    windowProps.Mode = Nutz::WindowMode::Windowed;

    m_Window = Nutz::Window::Create(windowProps);

    // CreateRenderer(API..., windowHandle)....
    Nutz::Renderer::Create(Nutz::RendererAPI::Vulkan, m_Window);

    m_Scene = Nutz::Scene::Create();
}

void EditorLayer::OnAttach()
{
    Nutz::AssetManager::Clear();

}

void EditorLayer::OnUpdate(Nutz::Timestep ts)
{
    m_Window->HandleEvents();
}
