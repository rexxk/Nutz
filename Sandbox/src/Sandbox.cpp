#include "Nutz.h"

#include "Core/Entrypoint.h"


#include <iostream>


class SandboxLayer : public Nutz::Layer
{
public:
	SandboxLayer(const std::string& layerName)
		: Layer(layerName)
	{

	}

	virtual void OnAttach() override
	{
		std::cout << "Attached layer: " << GetName() << "\n";
	}

	virtual void OnDetach() override
	{
		std::cout << "Detached layer: " << GetName() << "\n";
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


