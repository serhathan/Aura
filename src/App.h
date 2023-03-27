#pragma once
#include"pch.h"
#include <Window.h>
#include "Device.h"
#include <GameObject.h>
#include "Renderer.h"
#include "Descriptors.h"
#include "FrameInfo.h"
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

	private:
		void loadGameObjects();

		Window m_window {"Aura",WITDH,HEIGHT};
		Device m_device {m_window};
		Renderer m_renderer{m_window,m_device};
		std::unique_ptr<DescriptorPool> m_globalPool;
		GameObject::Map m_gameObjects;
	};
}

