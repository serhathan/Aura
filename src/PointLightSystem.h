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
		PointLightSystem(Device& device, VkRenderPass m_renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem& c) = delete;
		PointLightSystem& operator=(const PointLightSystem& c) = delete;
		void Render(FrameInfo& frameInfo);

		void Update(FrameInfo& frameInfo, GlobalUBO& ubo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass m_renderPass);

		Device& m_device;
		std::unique_ptr<Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	};
}
