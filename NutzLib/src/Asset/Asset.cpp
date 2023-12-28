#include "nutz_pch.h"
#include "Asset.h"



namespace Nutz
{


	Ref<Asset> Asset::Create(AssetType type, const std::string& path)
	{
		return CreateRef<Asset>(type, path);
	}


	Asset::Asset(AssetType type, const std::string& path)
	{
		m_Metadata.Type = type;
		m_Metadata.Path = path;
	}




}
