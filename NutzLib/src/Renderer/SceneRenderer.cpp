#include "nutz_pch.h"
#include "SceneRenderer.h"



namespace Nutz
{


	SceneRenderer::SceneRenderer()
	{
		{
			PipelineProperties props;
			props.Shader = ShaderLibrary::Get("basic");

			m_BasicPipeline = Pipeline::Create(props);
		}

	}

	SceneRenderer::~SceneRenderer()
	{
		Shutdown();
	}

	void SceneRenderer::Shutdown()
	{
		if (m_BasicPipeline != nullptr)
		{
			m_BasicPipeline->Shutdown();
			m_BasicPipeline = nullptr;
		}
	}



}
