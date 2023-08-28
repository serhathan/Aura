#pragma once
#include <FrameInfo.h>
#include <Pipeline.h>

namespace Aura {
	class TextureSubSystem
	{
	public:
		TextureSubSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~TextureSubSystem();

		TextureSubSystem(const TextureSubSystem& c) = delete;
		TextureSubSystem& operator=(const TextureSubSystem& c) = delete;

		void RenderGameObjects(FrameInfo& frameInfo);
		void Update(FrameInfo& frameInfo, UniformBufferObject& ubo);
	private:
		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);

		Device& m_device;
		std::unique_ptr<Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;
	};
}
