#pragma once


#include <vulkan/vulkan.h>



namespace Nutz
{




	class VulkanPhysicalDevice
	{
		struct QueueFamilyIndices
		{
			int32_t Graphics = -1;
			int32_t Compute = -1;
			int32_t Transfer = -1;
		};

	public:
		friend class VulkanDevice;

		static Ref<VulkanPhysicalDevice> Select(VkInstance instance);

		VulkanPhysicalDevice(VkInstance instance);
		~VulkanPhysicalDevice();

		const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

		VkPhysicalDevice GetVulkanPhysicalDevice() { return m_PhysicalDevice; }

	private:
		QueueFamilyIndices GetQueueFamilyIndices(int queueFlags);
		VkFormat FindDepthFormat();

		bool IsExtensionSupported(const std::string& extensionName) const;

	private:
		VkPhysicalDevice m_PhysicalDevice = nullptr;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;

		QueueFamilyIndices m_QueueFamilyIndices;

		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;
		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;
		std::vector<std::string> m_SupportedExtensions;

	};



	class VulkanDevice
	{
	public:
		static Ref<VulkanDevice> Create(Ref<VulkanPhysicalDevice> physicalDevice);

		VulkanDevice(Ref<VulkanPhysicalDevice> physicalDevice);
		~VulkanDevice();

		void Shutdown();

		VkDevice GetVulkanDevice() { return m_Device; }

	private:
		VkDevice m_Device = nullptr;

		VkQueue m_GraphicsQueue = nullptr;
		VkQueue m_ComputeQueue = nullptr;
	};


}
