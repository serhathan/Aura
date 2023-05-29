#pragma once
#include"pch.h"
#include <Pipeline.h>
#include "Device.h"
#include <GameObject.h>
#include <Camera.h>
#include "FrameInfo.h"
namespace Aura {

	struct PushConstantDataCubemap
	{
		glm::mat4 modelMatrix{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};

	class CubemapRenderSystem
	{
	public:
		CubemapRenderSystem(Device& device, VkRenderPass renderPass, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);
		~CubemapRenderSystem();

		CubemapRenderSystem(const CubemapRenderSystem& c) = delete;
		CubemapRenderSystem& operator=(const CubemapRenderSystem& c) = delete;

		void RenderGameObjects(FrameInfo& frameInfo);

	private:

		void CreatePipelineLayout(std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);
		void CreatePipeline(VkRenderPass renderPass);

		Device& m_device;
		std::unique_ptr<Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	};
}

