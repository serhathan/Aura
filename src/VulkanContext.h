#pragma once
#include <vulkan/vulkan.h>
#include <pch.h>
#include <Window.h>

namespace Aura {

	const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};


	class VulkanContext
	{
	public:
		VulkanContext(){}
		~VulkanContext(){}

		//static VkPhysicalDevice GetPhysicalDevice()  { return ; }
		//const VkDevice& GetLogicalDevice() const { return ; }
		
		void CleanUp(){}
	

	};

}
