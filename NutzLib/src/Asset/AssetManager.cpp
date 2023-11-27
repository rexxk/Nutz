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
				LOG_TRACE("Importing texture file ({})", extension);

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

	}


}
