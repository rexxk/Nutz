#pragma once

#include "Asset.h"



namespace Nutz
{


	class AssetManager
	{
	public:
		AssetManager() = default;

		static void Clear();


		static void LoadAsset(const std::filesystem::path& path);



	private:
		static void ImportObjectAsset(const std::filesystem::path& path);


	private:

		inline static std::vector<Ref<Asset>> s_AssetList;

	};


}
