#pragma once
#include "pch.h"
#include "GLFW\glfw3.h"
#include "SwapChain.h"
namespace Aura {
    class UI
    {
    public:
        UI(GLFWwindow* window, VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, uint32_t graphicsQueueFamily, VkQueue graphicsQueue, VkRenderPass renderPass, uint32_t subpass);

        void beginFrame();
        void endFrame(VkCommandBuffer commandBuffer);
        void cleanup(VkDevice device);

    private:
        static void check_vk_result(VkResult result);

        VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
        void endSingleTimeCommands(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer);
        void CreateRenderPass(VkDevice device);

        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        VkRenderPass m_imguiRenderPass;
        VkDescriptorPool descriptorPool;
        uint32_t minImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
        uint32_t imageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
        VkCommandPool commandPool;
    };

}
