#pragma once
#include "pch.h"
#include <Device.h>
#include "Buffer.h"
namespace Aura {
    struct PrimitiveCubemap
    {
        ~PrimitiveCubemap(){}
        uint32_t firstVertex;
        uint32_t vertexCount;
        VkDescriptorSet descriptorSet;
    };

    class Cubemap
    {
    public:
        Cubemap(Device& device, bool nearestFilter);
        ~Cubemap();

        Cubemap(const Cubemap& c) = delete;
        Cubemap(const Cubemap&& c) = delete;
        Cubemap& operator=(const Cubemap&& c) = delete;
        Cubemap& operator=(const Cubemap& c) = delete;

        VkImageView GetTextureImageView() const { return m_imageView; }
        VkSampler GetTextureSampler() const { return m_sampler; }
        void LoadTexture(const std::vector<std::string>& fileNames);
        void UpdateDescriptor();
    public:
        VkDescriptorImageInfo imageDescriptor;

    private:
        void CreateTextureImage(const std::vector<std::string>& fileNames);
        void CreateTextureImageView();
        void CreateTextureSampler();
        void GenerateMipmaps();

    private:
        static constexpr uint32_t NUMBER_OF_CUBEMAP_IMAGES = 6;

        bool m_nearestFilter;

        int m_width, m_height;
        int m_texChannels;
        std::unique_ptr<Buffer> m_stagingBuffer;
        uint32_t m_mipLevels = 1;

        Device& m_device;

        VkImage m_image;
        VkImageView m_imageView;
        VkSampler m_sampler;
        VkDeviceMemory m_imageMemory;
        VkFormat m_imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageLayout m_imageLayout;

    };
}