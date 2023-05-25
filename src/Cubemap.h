#pragma once
#include "pch.h"
#include <Device.h>

namespace Aura {
	class Cubemap
	{
	public:
		Cubemap(Device& device,bool nearestFilter);
		~Cubemap();

		Cubemap(const Cubemap& c) = delete;
		Cubemap(const Cubemap&& c) = delete;
		Cubemap& operator=(const Cubemap&& c) = delete;
		Cubemap& operator=(const Cubemap& c) = delete;

        VkImageView GetTextureImageView() const { return m_imageView; }
        VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }
        VkSampler GetTextureSampler() const { return m_sampler; }
        void LoadTexture(std::vector<std::string>& filePaths);
        void UpdateDescriptor();
    public:
        VkDescriptorImageInfo imageDescriptor;

    private:
        void CreateTextureImage(std::vector<std::string>& filePaths);
        void CreateTextureImageView();
        void CreateTextureSampler();
        void GenerateMipmaps();

    private:
        static constexpr uint32_t NUMBER_OF_CUBEMAP_IMAGES = 6;

        bool m_nearestFilter;

        int m_width, m_height;
        int m_texChannels;
        uint32_t m_mipLevels;

        Device& m_device;

        VkImage m_image;
        VkImageView m_imageView;
        VkSampler m_sampler;
        VkDeviceMemory m_imageMemory;
        VkFormat m_imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageLayout m_imageLayout;
        VkDescriptorSet m_descriptorSet;
	};
}

