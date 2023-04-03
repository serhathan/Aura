#pragma once
#include "pch.h"
#include "GLFW\glfw3.h"
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

        VkDescriptorPool descriptorPool;
        uint32_t minImageCount = 2;
        uint32_t imageCount = 2;
        VkCommandPool commandPool;
    };

}
