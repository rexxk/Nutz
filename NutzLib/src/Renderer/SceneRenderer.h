#pragma once

#include "Pipeline.h"


namespace Nutz
{

	class SceneRenderer
	{
	public:
		SceneRenderer();
		virtual ~SceneRenderer();

		void Shutdown();

	private:

		Ref<Pipeline> m_BasicPipeline = nullptr;

	};



}
