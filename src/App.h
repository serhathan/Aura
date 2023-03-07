#pragma once
#include"pch.h"
#include <Window.h>
#include <Pipeline.h>
#include "Device.h"
#include <SwapChain.h>
#include <Model.h>
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
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeComandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		bool m_Running = true;
		Window m_window {"Aura",WITDH,HEIGHT};
		Device device {m_window};
		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline ;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<Model> model;
	};
}

