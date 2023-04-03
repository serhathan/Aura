#pragma once
#include "pch.h"
#include <Buffer.h>
#include "glm\glm.hpp"
#include <imgui\imgui.h>
namespace Aura {
	class GUI
	{
	public:
		// UI params are set via push constants
		struct PushConstBlock {
			glm::vec2 scale;
			glm::vec2 translate;
		} pushConstBlock;

		GUI(Device& device);
		~GUI();

		void init(float width, float height);
		void setStyle(uint32_t index);
		//void initResources(VkRenderPass renderPass, VkQueue copyQueue, const std::string& shadersPath);

	private:
		// Vulkan resources for rendering the UI
		VkSampler sampler;
		Buffer vertexBuffer;
		Buffer indexBuffer;
		int32_t vertexCount = 0;
		int32_t indexCount = 0;
		VkDeviceMemory fontMemory = VK_NULL_HANDLE;
		VkImage fontImage = VK_NULL_HANDLE;
		VkImageView fontView = VK_NULL_HANDLE;
		VkPipelineCache pipelineCache;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		VkDescriptorPool descriptorPool;
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorSet descriptorSet;
		Device& device;
		VkPhysicalDeviceDriverProperties driverProperties = {};
		ImGuiStyle vulkanStyle;
		int selectedStyle = 0;

	};
}

