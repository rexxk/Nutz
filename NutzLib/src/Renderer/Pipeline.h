#pragma once



namespace Nutz
{


	struct PipelineProperties
	{
		float LineWidth = 1.0f;
	};



	class Pipeline
	{
	public:
		static Ref<Pipeline> Create(const PipelineProperties& pipelineProperties);


	};


}
