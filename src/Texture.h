#pragma once
#include "pch.h"
#include "Device.h"
#include "Buffer.h"

namespace Aura
{
    class Texture
    {
    public:
        Texture(Device& device);
        ~Texture();
        Texture(const Texture &c) = delete;
        Texture(const Texture &&c) = delete;
        Texture &operator=(const Texture &&c) = delete;
        Texture &operator=(const Texture &c) = delete;
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

        int m_width, m_height;
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