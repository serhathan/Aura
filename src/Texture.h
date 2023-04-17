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
        VkImageView GetTextureImageView() const { return m_textureImageView; }
        VkSampler GetTextureSampler() const { return m_textureSampler; }

        VkDescriptorPool GetDescPool() const { return m_descPool; }
        VkDescriptorSetLayout GetDescSetLayout()const { return m_descSetLayout; }
        std::vector<VkDescriptorSet> GetDescSets() const { return m_descSets; }
    private:
        void CreateTextureImage();
        void CreateTextureImageView();
        VkImageView CreateImageView(VkImage image, VkFormat format);
        void CreateTextureSampler();


        //void CreateDescriptorSetLayout();
        //void CreateDescriptorPool();
        //void CreateDescriptorSets();
        Device& m_device;

        VkImage m_textureImage;
        VkImageView m_textureImageView;
        VkSampler m_textureSampler;
        VkDeviceMemory m_textureImageMemory;

        VkDescriptorPool m_descPool;
        VkDescriptorSetLayout m_descSetLayout;
        std::vector<VkDescriptorSet> m_descSets;

    };
}