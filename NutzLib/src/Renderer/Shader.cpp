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
		LOG_CORE_TRACE("Loading shader {} to ShaderLibrary", filePath.filename().string());
		s_ShaderData.ShaderList[filePath.filename().string()] = Shader::Create(filePath);
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
		return s_ShaderData.ShaderList[name];
	}

}
