#pragma once

#include "Core/Base.h"
#include "Core/Application.h"
#include "Utils/Log.h"


extern Ref<Nutz::Application> CreateApp();


int main()
{
	Nutz::Log::Initialize();

	Ref<Nutz::Application> app = CreateApp();

	if (app)
	{
		app->Run();

		app->Shutdown();
	}
	else
	{
		return -1;
	}


	return 0;
}
