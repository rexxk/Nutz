#pragma once


#include "Renderer/Shader.h"


namespace Nutz
{

	enum class PrimitiveTopologyType
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		TriangleFan,
	};

	enum class PolygonFillType
	{
		Wireframe,
		Solid,
	};


	struct PipelineProperties
	{
		Ref<Shader> Shader = nullptr;

		PrimitiveTopologyType PrimitiveTopology = PrimitiveTopologyType::TriangleList;
		PolygonFillType PolygonFill = PolygonFillType::Solid;

		float LineWidth = 1.0f;
	};



	class Pipeline
	{
	public:
		static Ref<Pipeline> Create(const PipelineProperties& pipelineProperties);

		virtual ~Pipeline() {}

		virtual void Shutdown() = 0;

		virtual void Bind() = 0;
	};


}
