#pragma once
#include "pch.h"
#include "Device.h"
#include "FrameInfo.h"
#include "Pipeline.h"
#include "Descriptors.h"

namespace Aura {
	class RendererSubSystem
	{
	public:
		RendererSubSystem(Device& device, VkRenderPass renderPass)
			: m_device(device)
		{
			globalSetLayout = DescriptorSetLayout::Builder(m_device)
				.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
				.AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
				.Build();
		}

		virtual void Update(FrameInfo& frameInfo, GlobalUBO& ubo) = 0;
		virtual void Render(FrameInfo& frameInfo) = 0;


		std::unique_ptr<DescriptorSetLayout> globalSetLayout;

	protected:
		virtual void CreatePipelineLayout(std::vector<VkDescriptorSetLayout> descriptorSetLayouts) = 0;
		virtual void CreatePipeline(VkRenderPass renderPass) = 0;


	protected:

		Device& m_device;
		std::unique_ptr<Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	};
}