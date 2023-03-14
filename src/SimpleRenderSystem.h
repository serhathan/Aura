#pragma once
#include"pch.h"
#include <Pipeline.h>
#include "Device.h"
#include <GameObject.h>
#include <Camera.h>

namespace Aura {

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(Device &device,VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem& c) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem& c) = delete;
		void renderGameObjects(VkCommandBuffer commandBuffer,std::vector<GameObject> &gameObjects, Camera &camera);


	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		Device &device;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}

