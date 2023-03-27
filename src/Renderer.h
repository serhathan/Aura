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

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		bool isFrameInProgress() const { return isFrameStarted; }
		VkRenderPass getSwapChainRenderPass() const { return swapChain->GetRenderPass(); }

		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress!");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}
		float getAspectRation() const { return swapChain->GetExtentAspectRatio(); }
	private:
		void createCommandBuffers();
		void freeComandBuffers();
		void recreateSwapChain();

		Window& m_window;
		Device& device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool isFrameStarted {false};
	};
}

