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
	}

	virtual void OnDetach() override
	{
		LOG_TRACE("Detached layer: {}", GetName());
	}

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


