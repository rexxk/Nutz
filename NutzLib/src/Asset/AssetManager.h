#pragma once

#include "Asset.h"



namespace Nutz
{


	class AssetManager
	{
	public:
		AssetManager() = default;


		static void LoadAsset(const std::filesystem::path& path);



	private:

		std::vector<Ref<Asset>> m_AssetList;

	};


}
