#pragma once


#include "Renderer/Shader.h"

#include <vulkan/vulkan.h>




namespace Nutz
{

	struct SPIRVData
	{
		std::vector<uint32_t> Data;
		uint32_t DataSize;
	};


	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::filesystem::path& filePath);
		virtual ~VulkanShader();

		virtual void Shutdown() override;

		virtual void Reload() override;

	private:
		void GetShaderSources();

		void Compile();
		void Reflect();

	private:
		std::filesystem::path m_FilePath;

		std::unordered_map<ShaderDomain, std::string> m_ShaderSources;
		std::unordered_map<ShaderDomain, SPIRVData> m_SPIRVBuffers;
		std::vector<VkShaderModule> m_ShaderModules;

	};

}
