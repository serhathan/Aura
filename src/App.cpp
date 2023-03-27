#include "App.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "SimpleRenderSystem.h"
#include <Keyboard.h>
#include <numeric>
#include "PointLightSystem.h"
namespace Aura {


	App::App()
	{	
		DescriptorPool::Builder builder(device);

		builder.setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
		builder.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,SwapChain::MAX_FRAMES_IN_FLIGHT);
		
		globalPool = builder.build();
		

		loadGameObjects();
	}
	App::~App()
	{
	}

	void App::Run()
	{	
		std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<Buffer>(device,sizeof(GlobalUBO),1,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}

		DescriptorSetLayout::Builder builder(device);
		builder.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
		auto globalSetLayout = builder.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			DescriptorWriter(*globalSetLayout, *globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem(device, renderer.getSwapChainRenderPass(),globalSetLayout->getDescriptorSetLayout());
		PointLightSystem pointLight(device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

		Camera camera{};
		//camera.setViewTarget(glm::vec3(0.f,0.f,0.f),glm::vec3(0.0f,0.2f,1.f));
		camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = GameObject::createGameObject();
		viewerObject.transform.translation.z = -2.5f;
		Keyboard cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_window.ShouldCloseWindow())
		{
			m_window.OnUpdate();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime =
				std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.moveInPlaneXZ(m_window.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = renderer.getAspectRation();
			//camera.setOrthographicProjection(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
			camera.setPerspectiveProjection(glm::radians(45.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = renderer.beginFrame())
			{
				int frameIndex = renderer.getFrameIndex();

				FrameInfo frameInfo = {frameIndex,frameTime,commandBuffer,camera,globalDescriptorSets[frameIndex],gameObjects};

				//update
				GlobalUBO ubo {};

				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();

				pointLight.update(frameInfo,ubo);

				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				renderer.beginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.renderGameObjects(frameInfo);
				pointLight.render(frameInfo);

				renderer.endSwapChainRenderPass(commandBuffer);
				renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(device.device());
	}

	std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset)
	{
		Builder modelBuilder{};
		modelBuilder.vertices = {
			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		};
		for (auto& v : modelBuilder.vertices) {
			v.position += offset;
		}

		modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
								12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

		return std::make_unique<Model>(device, modelBuilder);
	}

	void App::loadGameObjects()
	{
		std::shared_ptr<Model> model = Model::createModelFormFile(device, "models/flat_vase.obj");

		auto flatVase = GameObject::createGameObject();
		flatVase.model = model;
		flatVase.transform.translation = { -0.5f,0.5f,0.f };
		flatVase.transform.scale = { 1.f,1.f,1.f };
		gameObjects.emplace(flatVase.getId(), std::move(flatVase));

		model = Model::createModelFormFile(device, "models/smooth_vase.obj");
		auto smoothVase = GameObject::createGameObject();
		smoothVase.model = model;
		smoothVase.transform.translation = { 0.5f,.5f,0.f };
		smoothVase.transform.scale = { 1.f,1.f,1.f };
		gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

		model = Model::createModelFormFile(device, "models/quad.obj");
		auto quad = GameObject::createGameObject();
		quad.model = model;
		quad.transform.translation = { 0.f,.5f,0.f };
		quad.transform.scale = { 3.f,1.f,3.f };
		gameObjects.emplace(quad.getId(), std::move(quad));

		std::vector<glm::vec3> lightColors{
			  {1.f, .1f, .1f},
			  {.1f, .1f, 1.f},
			  {.1f, 1.f, .1f},
			  {1.f, 1.f, .1f},
			  {.1f, 1.f, 1.f},
			  {1.f, 1.f, 1.f}  //
		};

		model = createCubeModel(device, glm::vec3(1.f));
		for (int i = 0; i < lightColors.size(); i++) {
			auto pointLight = GameObject::makePointLight(0.2f);
			pointLight.model = model;
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(
				glm::mat4(1.f),
				(i * glm::two_pi<float>()) / lightColors.size(),
				{ 0.f, -1.f, 0.f });
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
			gameObjects.emplace(pointLight.getId(), std::move(pointLight));
		}
		
	}
}