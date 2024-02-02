#include "nutz_pch.h"
#include "VulkanShader.h"
#include "VulkanContext.h"

#include "Core/Application.h"

#include <glslang/SPIRV/GlslangToSpv.h>



namespace Nutz
{


	ShaderDomain StringToShaderDomain(const std::string shaderDomain)
	{
		if (shaderDomain == "vertex")
			return ShaderDomain::Vertex;
		if (shaderDomain == "pixel" || shaderDomain == "fragment")
			return ShaderDomain::Fragment;
		if (shaderDomain == "compute")
			return ShaderDomain::Compute;

		return ShaderDomain::Vertex;
	}

	std::string ShaderDomainToString(ShaderDomain shaderDomain)
	{
		switch (shaderDomain)
		{
			case ShaderDomain::Vertex: return "vertex";
			case ShaderDomain::Fragment: return "fragment";
			case ShaderDomain::Compute: return "compute";
		}

		return "unknown";
	}

	EShLanguage ShaderDomainToEshLanguage(ShaderDomain shaderDomain)
	{
		switch (shaderDomain)
		{
			case ShaderDomain::Vertex: return EShLangVertex;
			case ShaderDomain::Fragment: return EShLangFragment;
			case ShaderDomain::Compute: return EShLangCompute;
		}

		return EShLangVertex;
	}

	VkShaderStageFlagBits ShaderDomainToStageFlag(ShaderDomain shaderDomain)
	{
		switch (shaderDomain)
		{
		case ShaderDomain::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		case ShaderDomain::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderDomain::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		}

		return VK_SHADER_STAGE_VERTEX_BIT;
	}
	
	void InitCompilerResources(TBuiltInResource& resources)
	{
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 64;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxCombinedShaderOutputResources = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;
		resources.maxMeshOutputVerticesNV = 256;
		resources.maxMeshOutputPrimitivesNV = 512;
		resources.maxMeshWorkGroupSizeX_NV = 32;
		resources.maxMeshWorkGroupSizeY_NV = 1;
		resources.maxMeshWorkGroupSizeZ_NV = 1;
		resources.maxTaskWorkGroupSizeX_NV = 32;
		resources.maxTaskWorkGroupSizeY_NV = 1;
		resources.maxTaskWorkGroupSizeZ_NV = 1;
		resources.maxMeshViewCountNV = 4;
		resources.limits.nonInductiveForLoops = 1;
		resources.limits.whileLoops = 1;
		resources.limits.doWhileLoops = 1;
		resources.limits.generalUniformIndexing = 1;
		resources.limits.generalAttributeMatrixVectorIndexing = 1;
		resources.limits.generalVaryingIndexing = 1;
		resources.limits.generalSamplerIndexing = 1;
		resources.limits.generalVariableIndexing = 1;
		resources.limits.generalConstantMatrixVectorIndexing = 1;

	}

    static uint32_t FormatSize(VkFormat format);

    
    VulkanShader::VulkanShader(const std::filesystem::path& filePath)
		: m_FilePath(filePath)
	{
		LOG_CORE_TRACE("Loading shader: {}", filePath.string());

		Reload();
	}

	VulkanShader::~VulkanShader()
	{

	}

	void VulkanShader::Shutdown()
	{
		VkDevice device = VulkanContext::Device();

		for (auto& iterator : m_ShaderModules)
		{
			auto& shaderModule = iterator.second;
			vkDestroyShaderModule(device, shaderModule, nullptr);
			shaderModule = nullptr;
		}

		m_ShaderModules.clear();
	}

	void VulkanShader::Reload()
	{
		GetShaderSources();

		Compile();

		Reflect();


	}

	void VulkanShader::GetShaderSources()
	{
		std::ifstream fs(m_FilePath.c_str(), std::ios::in | std::ios::binary);

		if (!fs.is_open())
		{
			LOG_CORE_ERROR("Failed to load {}", m_FilePath.string());
			return;
		}

		uint32_t fileLength = 0;
		fs.seekg(0, fs.end);
		fileLength = (uint32_t)fs.tellg();
		fs.seekg(0, fs.beg);

		LOG_CORE_TRACE("Shader file size: {}", fileLength);

		std::string filebuffer;
		filebuffer.resize(fileLength + 1);
		fs.read((char*)filebuffer.data(), (uint32_t)fileLength);
		filebuffer[fileLength] = 0;

		fs.close();

		size_t location = 0;
		size_t offset = 0;

		std::string typeString = "#type";

		while ((location = filebuffer.find(typeString, offset)) != std::string::npos)
		{
			size_t endPosition = filebuffer.find("\r\n", location);
			std::string domainString = filebuffer.substr(location + (uint32_t)typeString.length() + 1, endPosition - (location + (uint32_t)typeString.length() + 1));

			size_t shaderStart = endPosition + 1;
			size_t shaderEnd = filebuffer.find(typeString, endPosition);

			std::string shaderSource;

			if (shaderEnd == std::string::npos)
				shaderSource = filebuffer.substr(shaderStart);
			else
				shaderSource = filebuffer.substr(shaderStart, shaderEnd - shaderStart);

			m_ShaderSources[StringToShaderDomain(domainString)] = shaderSource;

			offset = endPosition;
		}
	}

	void VulkanShader::Compile()
	{
		glslang::InitializeProcess();

		TBuiltInResource resources = {};
		InitCompilerResources(resources);

		for (auto& sourceEntry : m_ShaderSources)
		{
			ShaderDomain shaderDomain = sourceEntry.first;
			std::string& shaderSource = sourceEntry.second;

			LOG_CORE_TRACE("Compiling {} shader", ShaderDomainToString(shaderDomain));

			EShLanguage stage = ShaderDomainToEshLanguage(shaderDomain);
			glslang::TShader shader(stage);
			glslang::TProgram program;

			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

			const char* shaderStrings[] = { (const char*)shaderSource.data() };
			shader.setStrings(shaderStrings, 1);

			if (!shader.parse(&resources, 100, false, messages))
			{
				LOG_CORE_ERROR("Shader compiler: {}", shader.getInfoLog());
				LOG_CORE_ERROR(" - {}", shader.getInfoDebugLog());
				return;
			}

			program.addShader(&shader);

			if (!program.link(messages))
			{
				LOG_CORE_ERROR("Shader compiler: {}", program.getInfoLog());
				LOG_CORE_ERROR(" - {}", program.getInfoDebugLog());
				return;
			}

			std::vector<uint32_t> spirvCode;
			glslang::GlslangToSpv(*program.getIntermediate(stage), spirvCode);

			SPIRVData spirvData;
			spirvData.Data = spirvCode;
			spirvData.DataSize = (uint32_t)spirvCode.size() * sizeof(uint32_t);

			VkShaderModuleCreateInfo moduleCreateInfo = {};
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.pCode = spirvCode.data();
			moduleCreateInfo.codeSize = (uint32_t)spirvCode.size() * sizeof(uint32_t);

			VkShaderModule shaderModule = nullptr;

			VkDevice device = VulkanContext::Device();

			if (vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
			{
				LOG_CORE_ERROR("Failed to create shader module");
				return;
			}

			m_ShaderModules[shaderDomain] = shaderModule;
			m_SPIRVBuffers[shaderDomain] = spirvData;
		}


		glslang::FinalizeProcess();
	}

	void VulkanShader::Reflect()
	{
		for (auto& iterator : m_SPIRVBuffers)
		{
			ShaderDomain shaderDomain = iterator.first;
			SPIRVData spirvData = iterator.second;

			LOG_CORE_TRACE("Reflect of {} shader", ShaderDomainToString(shaderDomain));

			SpvReflectShaderModule shaderModule;
			SpvReflectResult result = spvReflectCreateShaderModule(spirvData.DataSize, spirvData.Data.data(), &shaderModule);

			if (result != SPV_REFLECT_RESULT_SUCCESS)
			{
				LOG_CORE_ERROR("Shader reflection failed");
				return;
			}

			uint32_t inputVariableCount = 0;
			result = spvReflectEnumerateInputVariables(&shaderModule, &inputVariableCount, nullptr);

			m_ReflectionData[shaderDomain].InputVariables.resize(inputVariableCount);
			result = spvReflectEnumerateInputVariables(&shaderModule, &inputVariableCount, m_ReflectionData[shaderDomain].InputVariables.data());

			if (inputVariableCount > 0)
			{
				BuildInputAssemblyStateCreateInfo(shaderDomain);
			}

			for (auto& inputVariable : m_ReflectionData[shaderDomain].InputVariables)
			{
				LOG_CORE_TRACE(" - Input variable: {} @{}", inputVariable->name, inputVariable->location);
			}


			uint32_t descriptorSetCount = 0;
			spvReflectEnumerateDescriptorSets(&shaderModule, &descriptorSetCount, nullptr);

			m_ReflectionData[shaderDomain].DescriptorSets.resize(descriptorSetCount);
			spvReflectEnumerateDescriptorSets(&shaderModule, &descriptorSetCount, m_ReflectionData[shaderDomain].DescriptorSets.data());

			for (auto& descriptorSet : m_ReflectionData[shaderDomain].DescriptorSets)
			{
				LOG_CORE_TRACE(" - Descriptor set: {}, binding count: {}", descriptorSet->set, descriptorSet->binding_count);

				for (uint32_t i = 0; i < (uint32_t)descriptorSet->binding_count; i++)
				{
					LOG_CORE_TRACE("   Binding: {} - {}", descriptorSet->bindings[i]->name, descriptorSet->bindings[i]->binding);
				}
			}

			uint32_t descriptorBindingsCount = 0;
			spvReflectEnumerateDescriptorBindings(&shaderModule, &descriptorBindingsCount, nullptr);

			m_ReflectionData[shaderDomain].DescriptorBindings.resize(descriptorBindingsCount);
			spvReflectEnumerateDescriptorBindings(&shaderModule, &descriptorBindingsCount, m_ReflectionData[shaderDomain].DescriptorBindings.data());

			for (auto& descriptorBinding : m_ReflectionData[shaderDomain].DescriptorBindings)
			{
				LOG_CORE_TRACE(" - Descriptor binding: {}", descriptorBinding->name);

				for (uint32_t i = 0; i < (uint32_t)descriptorBinding->block.member_count; i++)
				{
					LOG_CORE_TRACE("   Name: {}", descriptorBinding->block.members[i].name);
				}
			}

			if (descriptorSetCount > 0)
			{
				BuildDescriptorSets(shaderDomain, shaderModule);
			}

			uint32_t outputVariableCount = 0;
			spvReflectEnumerateOutputVariables(&shaderModule, &outputVariableCount, nullptr);

			m_ReflectionData[shaderDomain].OutputVariables.resize(outputVariableCount);
			spvReflectEnumerateOutputVariables(&shaderModule, &outputVariableCount, m_ReflectionData[shaderDomain].OutputVariables.data());

			for (auto& outputVariable : m_ReflectionData[shaderDomain].OutputVariables)
			{
				LOG_CORE_TRACE(" - Output variable: {} @{}", outputVariable->name, outputVariable->location);
			}

			uint32_t pushConstantBlocksCount = 0;
			spvReflectEnumeratePushConstantBlocks(&shaderModule, &pushConstantBlocksCount, nullptr);

			m_ReflectionData[shaderDomain].PushConstantBlocks.resize(pushConstantBlocksCount);
			spvReflectEnumeratePushConstantBlocks(&shaderModule, &pushConstantBlocksCount, m_ReflectionData[shaderDomain].PushConstantBlocks.data());

			BuildPushConstantRanges(shaderDomain);

			for (auto& pushConstantBlock : m_ReflectionData[shaderDomain].PushConstantBlocks)
			{
				LOG_CORE_TRACE(" - Push constant block: {}", pushConstantBlock->name);

				for (uint32_t i = 0; i < (uint32_t)pushConstantBlock->member_count; i++)
				{
					LOG_CORE_TRACE("   Name: {}", pushConstantBlock->members[i].name);
				}
			}


			spvReflectDestroyShaderModule(&shaderModule);

		}


	}


	void VulkanShader::BuildInputAssemblyStateCreateInfo(ShaderDomain shaderDomain)
	{
		InputLayoutData& layoutData = m_InputLayouts[shaderDomain];

		VkVertexInputBindingDescription &bindingDescription = layoutData.BindingDescription;
		bindingDescription.binding = 0;
		bindingDescription.stride = 0;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription>& attributeDescriptions = layoutData.AttributeDescriptions;

		for (auto& var : m_ReflectionData[shaderDomain].InputVariables)
		{
			if (var->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN)
				continue;

			VkVertexInputAttributeDescription description = {};
			description.location = var->location;
			description.binding = bindingDescription.binding;
			description.format = (VkFormat)var->format;
			description.offset = 0;

			attributeDescriptions.push_back(description);
		}

		std::sort(attributeDescriptions.begin(), attributeDescriptions.end(), [](const VkVertexInputAttributeDescription& a, const VkVertexInputAttributeDescription& b)
			{
				return a.location < b.location;
			});

		for (auto& attribute : attributeDescriptions)
		{
			uint32_t formatSize = FormatSize(attribute.format);
			attribute.offset = bindingDescription.stride;
			bindingDescription.stride += formatSize;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		m_VertexInputStateCreateInfos[shaderDomain] = vertexInputStateCreateInfo;
	}

	void VulkanShader::BuildPushConstantRanges(ShaderDomain shaderDomain)
	{

		for (auto& pushConstantBlock : m_ReflectionData[shaderDomain].PushConstantBlocks)
		{
			VkPushConstantRange pushConstantRange = {};

			pushConstantRange.offset = pushConstantBlock->offset;
			pushConstantRange.size = pushConstantBlock->size;
			pushConstantRange.stageFlags = ShaderDomainToStageFlag(shaderDomain);

			m_PushConstants.push_back(pushConstantRange);
		}


	}

	void VulkanShader::BuildDescriptorSets(ShaderDomain shaderDomain, const SpvReflectShaderModule& shaderModule)
	{
		std::vector<DescriptorSetLayoutData> setLayouts(m_ReflectionData[shaderDomain].DescriptorSets.size(), DescriptorSetLayoutData());

		for (size_t i = 0; i < m_ReflectionData[shaderDomain].DescriptorSets.size(); i++)
		{
			const SpvReflectDescriptorSet& reflectSet = *(m_ReflectionData[shaderDomain].DescriptorSets[i]);
			DescriptorSetLayoutData& layout = setLayouts[i];

			layout.Bindings.resize(reflectSet.binding_count);

			for (uint32_t bindingIndex = 0; bindingIndex < reflectSet.binding_count; bindingIndex++)
			{
				const SpvReflectDescriptorBinding& reflectBinding = *(reflectSet).bindings[bindingIndex];
				VkDescriptorSetLayoutBinding& layoutBinding = layout.Bindings[bindingIndex];
				layoutBinding.binding = reflectBinding.binding;
				layoutBinding.descriptorType = static_cast<VkDescriptorType>(reflectBinding.descriptor_type);
				layoutBinding.descriptorCount = 1;

				for (uint32_t dimIndex = 0; dimIndex < reflectBinding.array.dims_count; dimIndex++)
				{
					layoutBinding.descriptorCount *= reflectBinding.array.dims[dimIndex];
				}
				
				layoutBinding.stageFlags = static_cast<VkShaderStageFlagBits>(shaderModule.shader_stage);
			}

			layout.SetNumber = reflectSet.set;
			layout.CreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layout.CreateInfo.bindingCount = reflectSet.binding_count;
			layout.CreateInfo.pBindings = layout.Bindings.data();
		}
	}


    static uint32_t FormatSize(VkFormat format)
    {
		switch (format)
		{
			case VK_FORMAT_UNDEFINED: return 0;
			case VK_FORMAT_R4G4_UNORM_PACK8: return 1;
			case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return 2;
			case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return 2;
			case VK_FORMAT_R5G6B5_UNORM_PACK16: return 2;
			case VK_FORMAT_B5G6R5_UNORM_PACK16: return 2;
			case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return 2;
			case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return 2;
			case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return 2;
			case VK_FORMAT_R8_UNORM: return 1;
			case VK_FORMAT_R8_SNORM: return 1;
			case VK_FORMAT_R8_USCALED: return 1;
			case VK_FORMAT_R8_SSCALED: return 1;
			case VK_FORMAT_R8_UINT: return 1;
			case VK_FORMAT_R8_SINT: return 1;
			case VK_FORMAT_R8_SRGB: return 1;
			case VK_FORMAT_R8G8_UNORM: return 2;
			case VK_FORMAT_R8G8_SNORM: return 2;
			case VK_FORMAT_R8G8_USCALED: return 2;
			case VK_FORMAT_R8G8_SSCALED: return 2;
			case VK_FORMAT_R8G8_UINT: return 2;
			case VK_FORMAT_R8G8_SINT: return 2;
			case VK_FORMAT_R8G8_SRGB: return 2;
			case VK_FORMAT_R8G8B8_UNORM: return 3;
			case VK_FORMAT_R8G8B8_SNORM: return 3;
			case VK_FORMAT_R8G8B8_USCALED: return 3;
			case VK_FORMAT_R8G8B8_SSCALED: return 3;
			case VK_FORMAT_R8G8B8_UINT: return 3;
			case VK_FORMAT_R8G8B8_SINT: return 3;
			case VK_FORMAT_R8G8B8_SRGB: return 3;
			case VK_FORMAT_B8G8R8_UNORM: return 3;
			case VK_FORMAT_B8G8R8_SNORM: return 3;
			case VK_FORMAT_B8G8R8_USCALED: return 3;
			case VK_FORMAT_B8G8R8_SSCALED: return 3;
			case VK_FORMAT_B8G8R8_UINT: return 3;
			case VK_FORMAT_B8G8R8_SINT: return 3;
			case VK_FORMAT_B8G8R8_SRGB: return 3;
			case VK_FORMAT_R8G8B8A8_UNORM: return 4;
			case VK_FORMAT_R8G8B8A8_SNORM: return 4;
			case VK_FORMAT_R8G8B8A8_USCALED: return 4;
			case VK_FORMAT_R8G8B8A8_SSCALED: return 4;
			case VK_FORMAT_R8G8B8A8_UINT: return 4;
			case VK_FORMAT_R8G8B8A8_SINT: return 4;
			case VK_FORMAT_R8G8B8A8_SRGB: return 4;
			case VK_FORMAT_B8G8R8A8_UNORM: return 4;
			case VK_FORMAT_B8G8R8A8_SNORM: return 4;
			case VK_FORMAT_B8G8R8A8_USCALED: return 4;
			case VK_FORMAT_B8G8R8A8_SSCALED: return 4;
			case VK_FORMAT_B8G8R8A8_UINT: return 4;
			case VK_FORMAT_B8G8R8A8_SINT: return 4;
			case VK_FORMAT_B8G8R8A8_SRGB: return 4;
			case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_UINT_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SINT_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_UINT_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SINT_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_UINT_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SINT_PACK32: return 4;
			case VK_FORMAT_R16_UNORM: return 2;
			case VK_FORMAT_R16_SNORM: return 2;
			case VK_FORMAT_R16_USCALED: return 2;
			case VK_FORMAT_R16_SSCALED: return 2;
			case VK_FORMAT_R16_UINT: return 2;
			case VK_FORMAT_R16_SINT: return 2;
			case VK_FORMAT_R16_SFLOAT: return 2;
			case VK_FORMAT_R16G16_UNORM: return 4;
			case VK_FORMAT_R16G16_SNORM: return 4;
			case VK_FORMAT_R16G16_USCALED: return 4;
			case VK_FORMAT_R16G16_SSCALED: return 4;
			case VK_FORMAT_R16G16_UINT: return 4;
			case VK_FORMAT_R16G16_SINT: return 4;
			case VK_FORMAT_R16G16_SFLOAT: return 4;
			case VK_FORMAT_R16G16B16_UNORM: return 6;
			case VK_FORMAT_R16G16B16_SNORM: return 6;
			case VK_FORMAT_R16G16B16_USCALED: return 6;
			case VK_FORMAT_R16G16B16_SSCALED: return 6;
			case VK_FORMAT_R16G16B16_UINT: return 6;
			case VK_FORMAT_R16G16B16_SINT: return 6;
			case VK_FORMAT_R16G16B16_SFLOAT: return 6;
			case VK_FORMAT_R16G16B16A16_UNORM: return 8;
			case VK_FORMAT_R16G16B16A16_SNORM: return 8;
			case VK_FORMAT_R16G16B16A16_USCALED: return 8;
			case VK_FORMAT_R16G16B16A16_SSCALED: return 8;
			case VK_FORMAT_R16G16B16A16_UINT: return 8;
			case VK_FORMAT_R16G16B16A16_SINT: return 8;
			case VK_FORMAT_R16G16B16A16_SFLOAT: return 8;
			case VK_FORMAT_R32_UINT: return 4;
			case VK_FORMAT_R32_SINT: return 4;
			case VK_FORMAT_R32_SFLOAT: return 4;
			case VK_FORMAT_R32G32_UINT: return 8;
			case VK_FORMAT_R32G32_SINT: return 8;
			case VK_FORMAT_R32G32_SFLOAT: return 8;
			case VK_FORMAT_R32G32B32_UINT: return 12;
			case VK_FORMAT_R32G32B32_SINT: return 12;
			case VK_FORMAT_R32G32B32_SFLOAT: return 12;
			case VK_FORMAT_R32G32B32A32_UINT: return 16;
			case VK_FORMAT_R32G32B32A32_SINT: return 16;
			case VK_FORMAT_R32G32B32A32_SFLOAT: return 16;
			case VK_FORMAT_R64_UINT: return 8;
			case VK_FORMAT_R64_SINT: return 8;
			case VK_FORMAT_R64_SFLOAT: return 8;
			case VK_FORMAT_R64G64_UINT: return 16;
			case VK_FORMAT_R64G64_SINT: return 16;
			case VK_FORMAT_R64G64_SFLOAT: return 16;
			case VK_FORMAT_R64G64B64_UINT: return 24;
			case VK_FORMAT_R64G64B64_SINT: return 24;
			case VK_FORMAT_R64G64B64_SFLOAT: return 24;
			case VK_FORMAT_R64G64B64A64_UINT: return 32;
			case VK_FORMAT_R64G64B64A64_SINT: return 32;
			case VK_FORMAT_R64G64B64A64_SFLOAT: return 32;
			case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return 4;
			case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return 4;
		}

        return 0;
    }
}
