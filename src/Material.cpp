#include "Material.h"
#include "SwapChain.h"
namespace Aura {
	Material::Material(Device& device, DescriptorPool* descriptorPool, std::unique_ptr<DescriptorSetLayout> descriptorSetLayout) 
		: m_device(device),m_globalPool(descriptorPool) ,descriptorSetLayout(std::move(descriptorSetLayout))
	{

	}
	Material::~Material()
	{
	}
	inline std::shared_ptr<Material> Material::Create(Device& device, DescriptorPool* descriptorPool, std::unique_ptr<DescriptorSetLayout> descriptorSetLayout)
	{
		return std::make_shared<Material>(device, descriptorPool, std::move(descriptorSetLayout));
	}
	void Material::CreateDescriptorSet()
	{
		if (baseColorTexture == nullptr)
			return;

		descriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < descriptorSets.size(); i++)
		{
			DescriptorWriter(*descriptorSetLayout, *m_globalPool)
				.writeImage(0, &baseColorTexture->imageDescriptor)
				.Build(descriptorSets[i]);
		}
	}
}