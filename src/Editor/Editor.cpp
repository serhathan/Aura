#include "Editor.h"
#include <backends/imgui_impl_vulkan.h>

namespace Aura {
	Editor::Editor(Device& device) : Layer("EditorLayer"),m_device(device)
	{
		m_viewportTexture = std::make_unique<Texture>(device);
	}
	void Editor::OnAttach()
	{
	}
	void Editor::OnDetach()
	{
	}
	void Editor::OnUpdate(float ts)
	{
	}
	void Editor::OnImGuiRender()
	{
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("Merhaba Hocam.");

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Renderer ortalama %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::Begin("Viewport");


		
		VkDescriptorSet descSet = ImGui_ImplVulkan_AddTexture(m_viewportTexture->GetTextureSampler(), m_viewportTexture->GetTextureImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)descSet, ImVec2{viewportPanelSize.x, viewportPanelSize.y});


		ImGui::End();
	}
}