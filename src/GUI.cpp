#include "GUI.h"
#include <winuser.rh>

namespace Aura {
	GUI::GUI(Device& device) : device(device)
	{
		
		ImGui::CreateContext();

		//SRS - Set ImGui font and style scale factors to handle retina and other HiDPI displays
		ImGuiIO& io = ImGui::GetIO();
		//io.FontGlobalScale = example->UIOverlay.scale;
		ImGuiStyle& style = ImGui::GetStyle();
		//style.ScaleAllSizes(example->UIOverlay.scale);
	}

	GUI::~GUI()
	{
		ImGui::DestroyContext();
		// Release all Vulkan resources required for rendering imGui
		//vertexBuffer.destroy();
		//indexBuffer.destroy();
		vkDestroyImage(device.GetDevice(), fontImage, nullptr);
		vkDestroyImageView(device.GetDevice(), fontView, nullptr);
		vkFreeMemory(device.GetDevice(), fontMemory, nullptr);
		vkDestroySampler(device.GetDevice(), sampler, nullptr);
		vkDestroyPipelineCache(device.GetDevice(), pipelineCache, nullptr);
		vkDestroyPipeline(device.GetDevice(), pipeline, nullptr);
		vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr);
		vkDestroyDescriptorPool(device.GetDevice(), descriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(device.GetDevice(), descriptorSetLayout, nullptr);
	}

	// Initialize styles, keys, etc.
	void GUI::init(float width, float height)
	{
		// Color scheme
		vulkanStyle = ImGui::GetStyle();
		vulkanStyle.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
		vulkanStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
		vulkanStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
		vulkanStyle.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
		vulkanStyle.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

		setStyle(0);

		// Dimensions
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(width, height);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
#if defined(_WIN32)
		// If we directly work with os specific key codes, we need to map special key types like tab
		io.KeyMap[ImGuiKey_Tab] = VK_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Space] = VK_SPACE;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
#endif
	}

	void GUI::setStyle(uint32_t index)
	{
		switch (index)
		{
		case 0:
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style = vulkanStyle;
			break;
		}
		case 1:
			ImGui::StyleColorsClassic();
			break;
		case 2:
			ImGui::StyleColorsDark();
			break;
		case 3:
			ImGui::StyleColorsLight();
			break;
		}
	}

}