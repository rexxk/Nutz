#include "nutz_pch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/Renderer/Vulkan/VulkanShader.h"


namespace Nutz
{


	struct ShaderLibraryData
	{
		std::unordered_map<std::string, Ref<Shader>> ShaderList;
	};

	static ShaderLibraryData s_ShaderData;


	Ref<Shader> Shader::Create(const std::filesystem::path& filePath)
	{
		switch (Renderer::API())
		{
			case RendererAPIType::Vulkan: return CreateRef<VulkanShader>(filePath);
			case RendererAPIType::OpenGL: return nullptr;
		}

		return nullptr;
	}


	void ShaderLibrary::Load(const std::filesystem::path& filePath)
	{
		std::string filename;

		uint32_t offset = (uint32_t)filePath.string().find_last_of('/') + 1;

		if (offset == std::string::npos)
			offset = 0;

		uint32_t length = (uint32_t)filePath.string().find('.') - offset;

		filename = filePath.string().substr(offset, length);

		LOG_CORE_TRACE("Loading shader {} to ShaderLibrary", filename);
		s_ShaderData.ShaderList[filename] = Shader::Create(filePath);
	}

	void ShaderLibrary::Shutdown()
	{
		for (auto& iterator : s_ShaderData.ShaderList)
		{
			Ref<Shader> shader = iterator.second;

			shader->Shutdown();
		}

		s_ShaderData.ShaderList.clear();
	}

	bool ShaderLibrary::IsEmpty()
	{
		return s_ShaderData.ShaderList.size() == 0;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		for (auto& iterator : s_ShaderData.ShaderList)
		{
			auto& shaderName = iterator.first;

			if (name == shaderName)
			{
				return iterator.second;
			}
		}

		return nullptr;
	}

}
