#pragma once
#include"pch.h"
#include <Pipeline.h>
#include "Device.h"
#include <GameObject.h>
#include <Camera.h>
#include "FrameInfo.h"
namespace Aura {

	

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(Device &device,VkRenderPass m_renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem& c) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem& c) = delete;
		void renderGameObjects(FrameInfo& frameInfo);


	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass m_renderPass);

		Device &device;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}

