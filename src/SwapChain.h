#pragma once
#include "pch.h"
#include "Device.h"

namespace Aura {

    class SwapChain {
    public:
        static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

        SwapChain(Device& device, VkExtent2D windowExtent);
        SwapChain(Device& device, VkExtent2D windowExtent,std::shared_ptr<SwapChain>previous);

        ~SwapChain();

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        VkFramebuffer GetFrameBuffer(int index) { return m_swapChainFramebuffers[index]; }
        VkRenderPass GetRenderPass() { return m_renderPass; }
        VkImageView GetImageView(int index) { return m_swapChainImageViews[index]; }
        std::vector<VkImageView> GetImageViews() const  { return m_swapChainImageViews; }

        size_t GetImageCount() { return m_swapChainImages.size(); }
        VkFormat GetSwapChainImageFormat() { return m_swapChainImageFormat; }
        VkExtent2D GetSwapChainExtent() { return m_swapChainExtent; }
        uint32_t GetWidth() { return m_swapChainExtent.width; }
        uint32_t GetHeight() { return m_swapChainExtent.height; }

        float GetExtentAspectRatio() {
            return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height);
        }
        VkFormat FindDepthFormat();

        VkResult AcquireNextImage(uint32_t* imageIndex);
        VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

        bool CompareSwapFormat(const SwapChain &swapChain) const 
        {
            return swapChain.m_swapChainDepthFormat == m_swapChainDepthFormat && swapChain.m_swapChainImageFormat == m_swapChainImageFormat;
        }

    private:
        void Init();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateDepthResources();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateSyncObjects();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkFormat m_swapChainImageFormat;
        VkFormat m_swapChainDepthFormat;
        VkExtent2D m_swapChainExtent;

        std::vector<VkFramebuffer> m_swapChainFramebuffers;
        VkRenderPass m_renderPass;

        std::vector<VkImage> m_depthImages;
        std::vector<VkDeviceMemory> m_depthImageMemorys;
        std::vector<VkImageView> m_depthImageViews;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;

        Device& m_device;
        VkExtent2D m_windowExtent;

        VkSwapchainKHR m_swapChain;
        std::shared_ptr<SwapChain> m_oldSwapChain;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        std::vector<VkFence> m_imagesInFlight;
        size_t m_currentFrame = 0;
    };

}