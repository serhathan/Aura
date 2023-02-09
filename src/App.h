#pragma once
#include <Window.h>
#include"pch.h"
#include "VulkanContext.h"
namespace Aura {

	class App
	{
	public:
		App();
		~App();

		void Run();

	private:
		bool m_Running = true;
		Window *m_Window;
		VulkanContext *m_Contex;
	};
}

