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
		SimpleRenderSystem(Device &m_device,VkRenderPass m_renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem& c) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem& c) = delete;

		void RenderGameObjects(FrameInfo& frameInfo);

	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass m_renderPass);

		Device &m_device;
		std::unique_ptr<Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	};
}

