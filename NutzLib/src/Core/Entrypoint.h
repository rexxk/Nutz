#pragma once

#include "Core/Base.h"
#include "Core/Application.h"
#include "Utils/Log.h"
#include "Utils/Statistics.h"


extern Ref<Nutz::Application> CreateApp();


int main()
{
	Nutz::Log::Initialize();
	Nutz::Statistics::Initialize();

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

	Nutz::Statistics::Shutdown();

	return 0;
}
