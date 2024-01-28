#pragma once



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
		PrimitiveTopologyType PrimitiveTopology = PrimitiveTopologyType::TriangleList;
		PolygonFillType PolygonFill = PolygonFillType::Solid;

		float LineWidth = 1.0f;
	};



	class Pipeline
	{
	public:
		static Ref<Pipeline> Create(const PipelineProperties& pipelineProperties);

		virtual ~Pipeline() {}
	};


}