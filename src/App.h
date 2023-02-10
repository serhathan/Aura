#pragma once
#include"pch.h"
#include <Window.h>
#include "VulkanContext.h"
#include <Instance.h>
#include <Device.h>
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
		std::shared_ptr<Instance> m_Instance;
		std::shared_ptr<Device> m_Device;
		std::shared_ptr<VulkanContext> m_Context;

	};
}

