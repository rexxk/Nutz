#include "Core/Entrypoint.h"

#include "Core/Application.h"

#include <iostream>


class SandboxApp : public Nutz::Application
{
public:
	SandboxApp()
	{
		std::cout << "Running SandboxApp\n";
	}
};



Ref<Nutz::Application> CreateApp()
{
	return CreateRef<SandboxApp>();
}


