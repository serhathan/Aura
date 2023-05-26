#include "Material.h"
#include "RendererManager.h"
#include "SwapChain.h"
namespace Aura {
	Material::Material(Device& device, std::unique_ptr<DescriptorSetLayout> descriptorSetLayout) : m_device(device), descriptorSetLayout(std::move(descriptorSetLayout))
	{

	}
	Material::~Material()
	{
	}
	inline std::shared_ptr<Material> Material::Create(Device& device, std::unique_ptr<DescriptorSetLayout> descriptorSetLayout)
	{
		return std::make_shared<Material>(device,std::move(descriptorSetLayout));
	}
	void Material::CreateDescriptorSet()
	{
		if (baseColorTexture == nullptr)
			return;

		descriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < descriptorSets.size(); i++)
		{
			DescriptorWriter(*descriptorSetLayout, *RendererManager::Instance().globalPool)
				.writeImage(0, &baseColorTexture->imageDescriptor)
				.Build(descriptorSets[i]);
		}
	}
}