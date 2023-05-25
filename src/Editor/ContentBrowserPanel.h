#pragma once
#include "pch.h"
#include <filesystem>
#include <Texture.h>

namespace Aura {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(Device& device);

		void OnImGuiRender();
	private:
		std::filesystem::path m_BaseDirectory;// TODO: Set Base directory
		std::filesystem::path m_CurrentDirectory;

		std::shared_ptr<Texture> m_DirectoryIcon;
		std::shared_ptr<Texture> m_FileIcon;
		Device& m_device;
	};

}