#pragma once
#include "pch.h"
#include "Device.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "Descriptors.h"
namespace Aura {

	enum AlphaMode { ALPHAMODE_OPAQUE, ALPHAMODE_MASK, ALPHAMODE_BLEND };

	class Material
	{
	public:
		~Material();

		static inline std::shared_ptr<Material> Create(Device& device, DescriptorPool* descriptorPool, std::unique_ptr<DescriptorSetLayout> descriptorSetLayout);
		void CreateDescriptorSet();
	public:
		Texture* baseColorTexture = nullptr;
		glm::vec4 baseColorFactor = glm::vec4(1.0f);
		AlphaMode alphaMode = ALPHAMODE_OPAQUE;
		std::vector<VkDescriptorSet> descriptorSets;
		std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
	protected:
		Material(Device& device,DescriptorPool* descriptorPool, std::unique_ptr<DescriptorSetLayout> descriptorSetLayout);

	private:
		Device& m_device;
		DescriptorPool* m_globalPool;
	};
}
