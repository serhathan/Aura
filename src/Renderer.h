#pragma once
#include"pch.h"
#include <Window.h>
#include "Device.h"
#include <SwapChain.h>
namespace Aura {

	class Renderer
	{
	public:
		Renderer(Window& window, Device& device);
		~Renderer();

		Renderer(const Renderer& c) = delete;
		Renderer& operator=(const Renderer& c) = delete;

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

		bool IsFrameInProgress() const { return m_isFrameStarted; }
		VkRenderPass GetSwapChainRenderPass() const { return m_swapChain->GetRenderPass(); }

		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			assert(m_isFrameStarted && "Cannot get command buffer when frame not in progress!");
			return m_commandBuffers[m_currentFrameIndex];
		}

		int GetFrameIndex() const {
			assert(m_isFrameStarted && "Cannot get frame index when frame not in progress");
			return m_currentFrameIndex;
		}
		float GetAspectRatio() const { return m_swapChain->GetExtentAspectRatio(); }

		// Refactor it later
		VkFormat GetSwapChainImageFormat() const { return m_swapChain->GetSwapChainImageFormat(); }
		size_t GetSwapChainImageCount() { return m_swapChain->GetImageCount(); }
		std::vector<VkImageView> GetSwapChainImageViews() const { return m_swapChain->GetImageViews(); }
		VkExtent2D GetSwapChainExtent2D() const { return m_swapChain->GetSwapChainExtent(); }
	private:
		void CreateCommandBuffers();
		void FreeComandBuffers();
		void RecreateSwapChain();

		Window& m_window;
		Device& m_device;
		std::unique_ptr<SwapChain> m_swapChain;
		std::vector<VkCommandBuffer> m_commandBuffers;

		uint32_t m_currentImageIndex;
		int m_currentFrameIndex{ 0 };
		bool m_isFrameStarted {false};
	};
}

