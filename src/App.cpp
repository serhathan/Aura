#include "App.h"
#include "pch.h"

namespace Aura {


	App::App() 
	{
		m_Window =new Window("Aura",800,800);
		m_Contex = new VulkanContext(m_Window);
				
	}
	App::~App() 
	{
		delete m_Contex;
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

