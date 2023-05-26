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
	private:
		Device& m_device;
		std::unique_ptr<DescriptorPool> m_globalPool;

	};

}
