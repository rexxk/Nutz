#pragma once




namespace Nutz
{


	enum class FormatType
	{
		B8G8R8A8_UNORM,
		R8G8B8A8_UNORM,
	};

	enum class SampleCountType
	{
		Samples_1,
		Samples_2,
		Samples_4,
		Samples_8,
		Samples_16,
		Samples_32,
		Samples_64,
	};

	enum class AttachmentLoadOpType
	{
		Clear,
		Load,
		DontCare,
	};

	enum class AttachmentStoreOpType
	{
		None,
		Store,
		DontCare,
	};


	struct RenderPassSpecification
	{
		FormatType Format = FormatType::B8G8R8A8_UNORM;
		SampleCountType Samples = SampleCountType::Samples_1;

		AttachmentLoadOpType AttachmentLoadOp = AttachmentLoadOpType::Clear;
		AttachmentStoreOpType AttachmentStoreOp = AttachmentStoreOpType::Store;

		AttachmentLoadOpType StencilLoadOp = AttachmentLoadOpType::DontCare;
		AttachmentStoreOpType StencilStoreOp = AttachmentStoreOpType::DontCare;
	};


	class RenderPass
	{
	public:
		static Ref<RenderPass> Create(const RenderPassSpecification& renderPassSpecification);

		virtual void Shutdown() = 0;
	};


}
