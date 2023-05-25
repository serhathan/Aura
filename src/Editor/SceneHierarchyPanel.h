#pragma once
#include "pch.h"
#include <GameObject.h>

namespace Aura {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;


		void OnImGuiRender();

		GameObject GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(GameObject entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(GameObject entity);
		void DrawComponents(GameObject entity);
	private:
		GameObject m_SelectionContext;
	};

}