#pragma once


#include "Core/DataBuffer.h"


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
		UUID AssetID;
		std::string Path;
	};



	class Asset
	{
	public:

		static Ref<Asset> Create(AssetType type, const std::string& path);

		Asset() = default;
		Asset(AssetType type, const std::string& path);

		AssetMetadata& GetMetadata() { return m_Metadata; }

	private:
		AssetMetadata m_Metadata;

		DataBuffer m_DataBuffer;

	};




}
