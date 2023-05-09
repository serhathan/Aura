#include "Cubemap.h"
#include "stb_image.h"
#include <Buffer.h>
namespace Aura {
	Cubemap::Cubemap(Device& device, bool nearestFilter) 
		: m_device(device), m_nearestFilter(nearestFilter), m_mipLevels(1)
		
	{
	}
	Cubemap::~Cubemap()
	{
		vkDestroyImage(m_device.GetDevice(), m_image, nullptr);
		vkFreeMemory(m_device.GetDevice(), m_imageMemory, nullptr);
		vkDestroyImageView(m_device.GetDevice(), m_imageView, nullptr);
		vkDestroySampler(m_device.GetDevice(), m_sampler, nullptr);
	}

	void Cubemap::LoadTexture(std::string filePath)
	{
		CreateTextureImage(filePath);
		CreateTextureImageView();
		CreateTextureSampler();
		UpdateDescriptor();
	}

	void Cubemap::UpdateDescriptor()
	{
		imageDescriptor.sampler = m_sampler;
		imageDescriptor.imageView = m_imageView;
		imageDescriptor.imageLayout = m_imageLayout;
	}

	void Cubemap::CreateTextureImage(std::string& filePath)
	{
		VkDeviceSize imageSize;
		VkDeviceSize layerSize;
		void* data;
		uint64_t memAddress;

		for (int i = 0; i < NUMBER_OF_CUBEMAP_IMAGES; i++) {
			stbi_uc* pixels = stbi_load(filePath.c_str(), &m_width, &m_height, &m_texChannels, STBI_rgb_alpha);
			if (!pixels)
			{
				throw std::runtime_error("failed to load texture image!");
			}

			if (i == 0)
			{
				layerSize = m_width * m_height * m_texChannels;
				imageSize = m_width * m_height * 4;
				m_stagingBuffer = { m_device, imageSize, 1, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

				if (m_stagingBuffer.Map())
					throw std::runtime_error("failed to allocate vertex buffer memory!");
				data = m_stagingBuffer.GetMappedMemory();
				memAddress = reinterpret_cast<uint64_t>(data);
			}
			memcpy(reinterpret_cast<void*>(memAddress), static_cast<void*>(pixels), static_cast<size_t>(layerSize));			stagingBuffer.WriteToBuffer((void*)pixels, imageSize);
			stbi_image_free(pixels);
			memAddress += layerSize;

		}
		m_stagingBuffer.Unmap();
		m_mipLevels = std::floor(std::log2(std::max(m_width, m_height))) + 1;

		

		
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(m_width);
		imageInfo.extent.height = static_cast<uint32_t>(m_height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = m_mipLevels;
		imageInfo.arrayLayers = NUMBER_OF_CUBEMAP_IMAGES;//Cubemap has 6 faces
		imageInfo.format = m_imageFormat;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;// For Cubemap

		m_device.CreateImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_imageMemory);
		m_device.TransitionImageLayout(m_image, m_imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels,NUMBER_OF_CUBEMAP_IMAGES);
		m_device.CopyBufferToImage(m_stagingBuffer.GetBuffer(), m_image, static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height), NUMBER_OF_CUBEMAP_IMAGES);
		//m_device.TransitionImageLayout(m_image, m_imageFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		GenerateMipmaps();
		m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void Cubemap::CreateTextureImageView()
	{
		m_imageView = m_device.CreateImageView(m_image, m_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, NUMBER_OF_CUBEMAP_IMAGES,VK_IMAGE_VIEW_TYPE_CUBE);
	}

	void Cubemap::CreateTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = m_device.properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f; // Optional
		samplerInfo.maxLod = static_cast<float>(m_mipLevels);
		samplerInfo.mipLodBias = 0.0f; // Optional

		if (vkCreateSampler(m_device.GetDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	void Cubemap::GenerateMipmaps()
	{
		VkFormatProperties formatProperties{};
		vkGetPhysicalDeviceFormatProperties(m_device.GetPhysicalDevice(), m_imageFormat, &formatProperties);

		if (!formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) {
			throw std::runtime_error("Texture image format does not support linear blitting!");
		}

		VkCommandBuffer commandBuffer = m_device.BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = m_image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = m_width;
		int32_t mipHeight = m_height;

		for (uint32_t i = 1; i < m_mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
				m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;

		}

		barrier.subresourceRange.baseMipLevel = m_mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		m_device.EndSingleTimeCommands(commandBuffer);
	}

}