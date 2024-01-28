#include "nutz_pch.h"
#include "VulkanShader.h"
#include "VulkanContext.h"

#include "Core/Application.h"

#include <glslang/SPIRV/GlslangToSpv.h>

#include "SPIRV-Reflect/spirv_reflect.h"


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
		VkDevice device = std::dynamic_pointer_cast<VulkanContext>(Application::Get().GetWindow()->GetRendererContext())->GetDevice();

		for (auto& shaderModule : m_ShaderModules)
		{
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

			VkDevice device = std::dynamic_pointer_cast<VulkanContext>(Application::Get().GetWindow()->GetRendererContext())->GetDevice();

			if (vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
			{
				LOG_CORE_ERROR("Failed to create shader module");
				return;
			}

			m_ShaderModules.emplace_back(shaderModule);
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

			uint32_t variableCount = 0;
			result = spvReflectEnumerateInputVariables(&shaderModule, &variableCount, nullptr);

			std::vector<SpvReflectInterfaceVariable*> inputVariables(variableCount);
			result = spvReflectEnumerateInputVariables(&shaderModule, &variableCount, inputVariables.data());

			for (auto& inputVariable : inputVariables)
			{
				LOG_CORE_TRACE(" - Input variable: {} @{}", inputVariable->name, inputVariable->location);
			}

			uint32_t descriptorSetCount = 0;
			spvReflectEnumerateDescriptorSets(&shaderModule, &descriptorSetCount, nullptr);

			std::vector<SpvReflectDescriptorSet*> descriptorSets(descriptorSetCount);
			spvReflectEnumerateDescriptorSets(&shaderModule, &descriptorSetCount, descriptorSets.data());

			for (auto& descriptorSet : descriptorSets)
			{
				LOG_CORE_TRACE(" - Descriptor set: {}, binding count: {}", descriptorSet->set, descriptorSet->binding_count);
			}

			uint32_t descriptorBindingsCount = 0;
			spvReflectEnumerateDescriptorBindings(&shaderModule, &descriptorBindingsCount, nullptr);

			std::vector<SpvReflectDescriptorBinding*> descriptorBindings(descriptorBindingsCount);
			spvReflectEnumerateDescriptorBindings(&shaderModule, &descriptorBindingsCount, descriptorBindings.data());

			for (auto& descriptorBinding : descriptorBindings)
			{
				LOG_CORE_TRACE(" - Descriptor binding: {}", descriptorBinding->name);
			}

			uint32_t outputVariableCount = 0;
			spvReflectEnumerateOutputVariables(&shaderModule, &outputVariableCount, nullptr);

			std::vector<SpvReflectInterfaceVariable*> outputVariables(outputVariableCount);
			spvReflectEnumerateOutputVariables(&shaderModule, &outputVariableCount, outputVariables.data());

			for (auto& outputVariable : outputVariables)
			{
				LOG_CORE_TRACE(" - Output variable: {} @{}", outputVariable->name, outputVariable->location);
			}

			uint32_t pushConstantBlocksCount = 0;
			spvReflectEnumeratePushConstantBlocks(&shaderModule, &pushConstantBlocksCount, nullptr);

			std::vector<SpvReflectBlockVariable*> pushConstantBlocks(pushConstantBlocksCount);
			spvReflectEnumeratePushConstantBlocks(&shaderModule, &pushConstantBlocksCount, pushConstantBlocks.data());

			for (auto& pushConstantBlock : pushConstantBlocks)
			{
				LOG_CORE_TRACE(" - Push constant block: {}", pushConstantBlock->name);
			}


			spvReflectDestroyShaderModule(&shaderModule);

		}


	}

}
