#pragma once


#include "Renderer/Shader.h"

#include "SPIRV-Reflect/spirv_reflect.h"

#include <vulkan/vulkan.h>




namespace Nutz
{



	class VulkanShader : public Shader
	{
		struct SPIRVData
		{
			std::vector<uint32_t> Data;
			uint32_t DataSize;
		};

		struct ReflectionData
		{
			std::vector<SpvReflectInterfaceVariable*> InputVariables;
			std::vector<SpvReflectInterfaceVariable*> OutputVariables;

			std::vector<SpvReflectDescriptorSet*> DescriptorSets;
			std::vector<SpvReflectDescriptorBinding*> DescriptorBindings;

			std::vector<SpvReflectBlockVariable*> PushConstantBlocks;
		};

		struct InputLayoutData
		{
			VkVertexInputBindingDescription BindingDescription;
			std::vector<VkVertexInputAttributeDescription> AttributeDescriptions;
		};

	public:
		VulkanShader(const std::filesystem::path& filePath);
		virtual ~VulkanShader();

		virtual void Reload() override;
		virtual void Shutdown() override;

		VkPipelineVertexInputStateCreateInfo GetVertexInputStateCreateInfo(ShaderDomain shaderDomain) { return m_VertexInputStateCreateInfos[shaderDomain]; }

		const std::unordered_map<ShaderDomain, VkShaderModule>& GetShaderModules() const { return m_ShaderModules; }

	private:
		void GetShaderSources();

		void Compile();
		void Reflect();

		void BuildInputAssemblyStateCreateInfo(ShaderDomain shaderDomain);

	private:
		std::filesystem::path m_FilePath;

		std::unordered_map<ShaderDomain, std::string> m_ShaderSources;
		std::unordered_map<ShaderDomain, SPIRVData> m_SPIRVBuffers;
		std::unordered_map<ShaderDomain, VkShaderModule> m_ShaderModules;

		ReflectionData m_ReflectionData;

		std::unordered_map<ShaderDomain, InputLayoutData> m_InputLayouts;

		// Descriptor sets
		std::vector<VkDescriptorSet> m_DescriptorSets;

		// Input layout
		std::unordered_map<ShaderDomain, VkPipelineVertexInputStateCreateInfo> m_VertexInputStateCreateInfos;
	};

}
