#pragma once
#include "pch.h"
#include <VulkanContext.h>

namespace Aura {

	class Device
	{
	public:
		Device(VkInstance& instance);

		~Device();


		VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
		VkDevice GetLogicalDevice() const { return device; }

	private:

		void Init();
		void CleanUp();

		void pickPhysicalDevice();
		bool isDeviceSuitable(const VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkInstance instance;
	};

}