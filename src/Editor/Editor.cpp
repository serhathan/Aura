#include "Editor.h"

namespace Aura {
	Editor::Editor() : Layer("EditorLayer")
	{
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
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
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
	}
}