#pragma once
#include"pch.h"
#include <Pipeline.h>
#include "Device.h"
#include <GameObject.h>
#include <Camera.h>
#include <SimpleRenderSystem.h>

namespace Aura {


	class PointLightSystem
	{
	public:
		PointLightSystem(Device &device,VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem& c) = delete;
		PointLightSystem& operator=(const PointLightSystem& c) = delete;
		void render(FrameInfo& frameInfo);

		void update(FrameInfo& frameInfo, GlobalUBO& ubo);
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		Device &device;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}

