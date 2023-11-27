#pragma once



namespace Nutz
{


	enum class AssetType
	{
		Texture,
		Model,
		Sound,

	};


	struct AssetMetadata
	{
		AssetType Type;
		uint64_t AssetID;
		std::string Path;
	};



	class Asset
	{
	public:
		Asset() = default;

	};




}
