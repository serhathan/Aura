#include "App.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "SimpleRenderSystem.h"
namespace Aura {

	App::App() 
	{
		loadGameObjects();
	}
	App::~App() 
	{
	}

	void App::Run() 
	{
		SimpleRenderSystem simpleRenderSystem(device,renderer.getSwapChainRenderPass());
		while (!m_window.ShouldCloseWindow())
		{
			m_window.OnUpdate();
			if (auto commandBuffer = renderer.beginFrame())
			{
				renderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer,gameObjects);
				renderer.endSwapChainRenderPass(commandBuffer);
				renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(device.device());
	}

	void App::loadGameObjects()
	{
		std::vector<Vertex> vertices{ 
			{{0.0f, -0.5f},{1.0f,0.0f,0.0f}},
			{{0.5f, 0.5f},{0.0f,1.0f,0.0f} },
			{{-0.5f, 0.5f},{0.0f,0.0f,1.0f}}
		};
		auto model = std::make_shared<Model>(device, vertices);
		auto triangle = GameObject::createGameObject();
		triangle.model = model;
		triangle.color = {.1f,.8f,.1f};

		triangle.transform2D.translation.x = .2f;
		triangle.transform2D.scale.x = .5f;
		triangle.transform2D.rotation = 1.f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}


}

