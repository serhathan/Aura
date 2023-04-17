#pragma once
#include "pch.h"
#include "Layer.h"
#include <imgui/imgui.h>
namespace Aura {
	class Editor : public Layer
	{
	public:
		Editor();
		virtual ~Editor() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(float ts) override;
		virtual void OnImGuiRender() override;
	private:
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGuiIO& io = ImGui::GetIO();
	};
}
