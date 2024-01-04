#include "Nutz.h"

#include "Core/Entrypoint.h"


class SandboxLayer : public Nutz::Layer
{
public:
	SandboxLayer(const std::string& layerName)
		: Layer(layerName)
	{

	}

	virtual void OnAttach() override
	{ 
		LOG_TRACE("Attached layer: {}", GetName());

		Nutz::AssetManager::LoadAsset("assets/objects/cube.obj");
//		Nutz::AssetManager::LoadAsset("assets/textures/checkerboard.png");

		Nutz::AssetManager::DebugPrint();

		m_Scene = Nutz::Scene::Create();

	}

	virtual void OnDetach() override
	{
		LOG_TRACE("Detached layer: {}", GetName());
	}

	virtual void OnUpdate(Nutz::Timestep ts) override
	{

	}

private:
	Ref<Nutz::Scene> m_Scene;

};




class SandboxApp : public Nutz::Application
{
public:
	SandboxApp()
	{
		Ref<Nutz::Layer> layer = CreateRef<SandboxLayer>("Sandbox layer");
		AttachLayer(layer);

	}
};



Ref<Nutz::Application> CreateApp()
{
	return CreateRef<SandboxApp>();
}


