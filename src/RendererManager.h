#pragma once
#include "pch.h"
#include "RendererSubSystem.h"
#include "SimpleRenderSystem.h"
#include "Descriptors.h"
#include "Singleton.h"
namespace Aura {
	class RendererManager : public Singleton<RendererManager>
	{
	public:
		RendererManager(Device& device);

		std::vector<RendererSubSystem> rendererSubSystems;
		std::vector<std::unique_ptr<Buffer>> uboBuffers;
		std::vector<VkDescriptorSet> globalDescriptorSets;
		std::unique_ptr<DescriptorPool> globalPool;

	private:
		Device& m_device;
	};

}
