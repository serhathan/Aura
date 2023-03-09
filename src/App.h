#pragma once
#include"pch.h"
#include <Window.h>
#include "Device.h"
#include <GameObject.h>
#include "Renderer.h"

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
		void renderGameObjects(VkCommandBuffer commandBuffer);

		Window m_window {"Aura",WITDH,HEIGHT};
		Device device {m_window};
		Renderer renderer {m_window,device};
		std::vector<GameObject> gameObjects;
	};
}

