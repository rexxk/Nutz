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
    Nutz::Renderer::Create(Nutz::RendererAPIType::Vulkan);

    m_Scene = Nutz::Scene::Create();


    Nutz::ShaderLibrary::Load("assets/shaders/basic.shader");

//    Nutz::PipelineProperties pipelineProps;
//    Ref<Nutz::Pipeline> pipeline = Nutz::Pipeline::Create(pipelineProps);

}

void EditorLayer::OnAttach()
{
    Nutz::AssetManager::Clear();

}

void EditorLayer::OnUpdate(Nutz::Timestep ts)
{
    Nutz::Application::Get().GetWindow()->HandleEvents();
}
