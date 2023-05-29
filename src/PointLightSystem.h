#pragma once
#include"pch.h"
#include <GameObject.h>
#include <Camera.h>
#include "RendererSubSystem.h"
namespace Aura {
	class PointLightSystem : public RendererSubSystem
	{
	public:
		PointLightSystem(Device& device, VkRenderPass renderPass);
		PointLightSystem(Device& device, VkRenderPass renderPass, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem& c) = delete;
		PointLightSystem& operator=(const PointLightSystem& c) = delete;

		virtual void Render(FrameInfo& frameInfo);
		virtual void Update(FrameInfo& frameInfo, GlobalUBO& ubo) override;
	private:
		virtual void CreatePipelineLayout(std::vector<VkDescriptorSetLayout> descriptorSetLayouts) override;
		virtual void CreatePipeline(VkRenderPass m_renderPass) override;

	};
}