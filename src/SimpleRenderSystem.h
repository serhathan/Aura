#pragma once
#include"pch.h"
#include <Pipeline.h>
#include "Device.h"
#include <GameObject.h>
#include <Camera.h>

namespace Aura {

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
	};

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(Device &device,VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem& c) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem& c) = delete;
		void renderGameObjects(FrameInfo& frameInfo,std::vector<GameObject> &gameObjects);


	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		Device &device;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}

