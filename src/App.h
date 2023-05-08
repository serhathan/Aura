#pragma once
#include"pch.h"
#include <Window.h>
#include "Device.h"
#include <GameObject.h>
#include "Renderer.h"
#include "Descriptors.h"
#include "FrameInfo.h"
#include <UI.h>
#include "LayerStack.h"
namespace Aura {

	class App
	{
	public:
		App();
		~App();

		App(const App& c) = delete;
		App& operator=(const App& c) = delete;

		void Run();

		static constexpr uint32_t WITDH = 800;
		static constexpr uint32_t HEIGHT = 600;

		void PushOverlay(Layer* layer);
		void PushLayer(Layer* layer);
	private:
		void loadGameObjects();
		//void InitGUI();
		
	private:
		Window m_window {"Aura",WITDH,HEIGHT};
		Device m_device {m_window};
		Renderer m_renderer{m_window,m_device};
		std::unique_ptr<DescriptorPool> m_globalPool;
		GameObject::Map m_gameObjects;
		UI* m_ui = new UI{ m_window.GetGLFWWindow(),m_device.GetInstance(),m_device.GetPhysicalDevice(),m_device.GetDevice(),
			m_device.FindPhysicalQueueFamilies().graphicsFamily.value(),m_device.GetGraphicsQueue(),m_renderer.GetSwapChainRenderPass(),0 };
		LayerStack m_layerStack;


	};
}

