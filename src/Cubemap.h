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
        VkSampler GetTextureSampler() const { return m_sampler; }
        void LoadTexture(std::string filePath);
        void UpdateDescriptor();
    public:
        VkDescriptorImageInfo imageDescriptor;

    private:
        void CreateTextureImage(std::string& filePath);
        void CreateTextureImageView();
        void CreateTextureSampler();
        void GenerateMipmaps();

    private:
        static constexpr uint32_t NUMBER_OF_CUBEMAP_IMAGES = 6;

        bool m_nearestFilter;

        int m_width, m_height;
        int m_texChannels;
        Buffer m_stagingBuffer;
        uint32_t m_mipLevels;

        Device& m_device;

        VkImage m_image;
        VkImageView m_imageView;
        VkSampler m_sampler;
        VkDeviceMemory m_imageMemory;
        VkFormat m_imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageLayout m_imageLayout;

	};
}

