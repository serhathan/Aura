#pragma once
#include "pch.h"
#include <Device.h>
namespace Aura {

	struct PipelineConfigInfo {
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo& c) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo& c) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDesc{};
		std::vector<VkVertexInputAttributeDescription> attributeDesc{};

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass m_renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline
	{
	public:
		Pipeline() = default;
		Pipeline(Device& device,const std::string& vertexPath, const std::string& fragmentPath,const PipelineConfigInfo& configInfo);
		~Pipeline();

		Pipeline(const Pipeline& c) = delete;
		Pipeline& operator=(const Pipeline& c) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		static std::vector<char> ReadFile(const std::string& filePath);
		void CreateGraphicsPipeline(const std::string& vertexPath, const std::string& fragmentPath, const PipelineConfigInfo& configInfo);
		void CreateShaderModule(const std::vector<char>& code, VkShaderModule *shaderModule);

		Device& m_device;
		VkPipeline m_graphicsPipeline;
		VkShaderModule m_vertShaderModule;
		VkShaderModule m_fragShaderModule;

	};
}

