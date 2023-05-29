#include "CubemapRenderSystem.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Aura {


	CubemapRenderSystem::CubemapRenderSystem(Device& device, VkRenderPass renderPass, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts) : m_device(device)
	{
		CreatePipelineLayout(descriptorSetLayouts);
		CreatePipeline(renderPass);
	}
	CubemapRenderSystem::~CubemapRenderSystem()
	{
		vkDestroyPipelineLayout(m_device.GetDevice(), m_pipelineLayout, nullptr);
	}



	void CubemapRenderSystem::CreatePipelineLayout(std::vector<VkDescriptorSetLayout>& descriptorSetLayouts)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantDataCubemap);


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(m_device.GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void CubemapRenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_pipelineLayout;
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;

		pipelineConfig.depthStencilInfo.depthTestEnable = VK_FALSE;
		pipelineConfig.depthStencilInfo.depthWriteEnable = VK_FALSE;

		m_pipeline = std::make_unique<Pipeline>(m_device, "shaders/skybox.vert.spv", "shaders/skybox.frag.spv", pipelineConfig);
	}

	void CubemapRenderSystem::RenderGameObjects(FrameInfo& frameInfo)
	{
		m_pipeline->Bind(frameInfo.commandBuffer);

		std::vector<VkDescriptorSet> descriptorSets = { frameInfo.globalDescriptorSet };

		vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 2, descriptorSets.data(), 0, nullptr);


		for (auto& kv : frameInfo.gameObjects) {
			auto& obj = kv.second;
			if (obj.model == nullptr) continue;
			PushConstantDataCubemap push{};
			push.modelMatrix = obj.transform.mat4();
			push.normalMatrix = obj.transform.normalMatrix();

			vkCmdPushConstants(frameInfo.commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantDataCubemap), &push);
			obj.model->Bind(frameInfo.commandBuffer);
			obj.model->Draw(frameInfo.commandBuffer);
		}
	}
}
