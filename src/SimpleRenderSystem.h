#pragma once
#include"pch.h"
#include <GameObject.h>
#include <Camera.h>
#include "RendererSubSystem.h"
namespace Aura {
	class SimpleRenderSystem : public RendererSubSystem
	{
	public:
		SimpleRenderSystem(Device& device, VkRenderPass renderPass);
		SimpleRenderSystem(Device& device, VkRenderPass renderPass, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem& c) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem& c) = delete;

		virtual void Update(FrameInfo& frameInfo, GlobalUBO& ubo) override;
		virtual void Render(FrameInfo& frameInfo) override;
	private:
		virtual void CreatePipelineLayout(std::vector<VkDescriptorSetLayout> descriptorSetLayouts) override;
		virtual void CreatePipeline(VkRenderPass renderPass) override;

	};
}
