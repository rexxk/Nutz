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

    Nutz::Application::CreateWindow(windowProps);

    m_Scene = Nutz::Scene::Create();


    m_SceneRenderer = CreateRef<Nutz::SceneRenderer>();

}

void EditorLayer::OnAttach()
{
    Nutz::AssetManager::Clear();

}

void EditorLayer::OnDetach()
{
    m_SceneRenderer->Shutdown();

    Nutz::Renderer::Shutdown();
}


void EditorLayer::OnUpdate(Nutz::Timestep ts)
{
    Nutz::Application::Get().GetWindow()->HandleEvents();
}
