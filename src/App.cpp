#include "App.h"

namespace Aura {


	App::App() 
	{
		m_Window =new Window("Aura",800,800);

		m_Instance = std::make_shared<Instance>();

		m_Instance->GetVulkanInstance();

		m_Device = std::make_shared<Device>(m_Instance->GetVulkanInstance());
	}
	App::~App() 
	{
		delete m_Window;
		
	}

	void App::Run() 
	{
		while (!m_Window->ShouldCloseWindow())
		{
			m_Window->OnUpdate();
		}
	}
}

