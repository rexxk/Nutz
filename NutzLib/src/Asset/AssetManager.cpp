#include "nutz_pch.h"
#include "AssetManager.h"




namespace Nutz
{


	void AssetManager::LoadAsset(const std::filesystem::path& path)
	{

		if (path.has_extension())
		{
			std::string extension = path.extension().string();

			if (extension == ".obj" || extension == ".fbx")
			{
				ImportObjectAsset(path);

				return;
			}
			if (extension == ".jpg" || extension == ".png")
			{
				ImportTextureAsset(path);

				return;
			}
			else
			{
				LOG_ERROR("Not a valid asset extension ({})", extension);

				return;
			}

		}
		else
		{
			LOG_ERROR("Asset has no extension! Unable to distinguish the asset type.");
		}

	}

	void AssetManager::Clear()
	{
		s_AssetList.clear();
	}

	void AssetManager::ImportObjectAsset(const std::filesystem::path& path)
	{
		LOG_TRACE("Importing object file ({})", path.string());

		s_AssetList.emplace_back(Asset::Create(AssetType::Model, path.string()));
	}

	void AssetManager::ImportTextureAsset(const std::filesystem::path& path)
	{
		LOG_TRACE("Importing texture file ({})", path.string());

		s_AssetList.emplace_back(Asset::Create(AssetType::Texture, path.string()));
	}

	void AssetManager::DebugPrint()
	{
		for (auto& asset : s_AssetList)
		{
			auto& metadata = asset->GetMetadata();
			LOG_TRACE("Asset UUID: {}   - Type: {}  - Path: {}", (uint64_t)metadata.AssetID, (uint8_t)metadata.Type, metadata.Path);
		}
	}

}
