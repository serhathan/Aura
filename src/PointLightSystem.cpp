#include "PointLightSystem.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace Aura {

	struct PointLightPushConstants
	{
		glm::vec4 position{};
		glm::vec4 color{};
		float radius;
	};

	PointLightSystem::PointLightSystem(Device& device, VkRenderPass m_renderPass, VkDescriptorSetLayout globalSetLayout) : m_device(device)
	{
		CreatePipelineLayout(globalSetLayout);
		CreatePipeline(m_renderPass);
	}

	PointLightSystem::~PointLightSystem()
	{
		vkDestroyPipelineLayout(m_device.GetDevice(), m_pipelineLayout, nullptr);
	}

	void PointLightSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PointLightPushConstants);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(m_device.GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) !=VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

	}

	void PointLightSystem::CreatePipeline(VkRenderPass m_renderPass)
	{
		assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.attributeDesc.clear();
		pipelineConfig.bindingDesc.clear();

		pipelineConfig.renderPass = m_renderPass;
		pipelineConfig.pipelineLayout = m_pipelineLayout;
		m_pipeline = std::make_unique<Pipeline>(m_device, "shaders/pointLight.vert.spv", "shaders/pointLight.frag.spv", pipelineConfig);
	}

	void PointLightSystem::Render(FrameInfo& frameInfo)
	{
		m_pipeline->Bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);
		
		for (auto& kv : frameInfo.gameObjects) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr) continue;

			PointLightPushConstants push{};
			push.position = glm::vec4(obj.transform.translation, 1.f);
			push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
			push.radius = obj.transform.scale.x;

			vkCmdPushConstants(frameInfo.commandBuffer,m_pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,sizeof(PointLightPushConstants),&push);
			vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
		}
	}
	void PointLightSystem::Update(FrameInfo& frameInfo, GlobalUBO& ubo)
	{
		auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * frameInfo.frameTime, { 0.f, -1.f, 0.f });
		int lightIndex = 0;
		for (auto& kv : frameInfo.gameObjects) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr) continue;

			assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

			// update light position
			obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));

			// copy light to ubo
			ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
			ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);

			lightIndex += 1;
		}
		ubo.numLights = lightIndex;
	}
}

