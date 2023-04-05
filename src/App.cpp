#include "App.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "SimpleRenderSystem.h"
#include <Keyboard.h>
#include <numeric>
#include "PointLightSystem.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

namespace Aura {

	App::App()
	{
		DescriptorPool::Builder builder(m_device);

		builder.SetMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT);

		m_globalPool = builder.Build();

		loadGameObjects();

		//InitGUI();


	}
	App::~App()
	{
		m_ui.cleanup(m_device.GetDevice());
	}

	void App::Run()
	{
		std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<Buffer>(m_device, sizeof(GlobalUBO), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->Map();
		}

		DescriptorSetLayout::Builder builder(m_device);
		builder.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
		auto globalSetLayout = builder.Build();

		std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->DescriptorInfo();
			DescriptorWriter(*globalSetLayout, *m_globalPool).writeBuffer(0, &bufferInfo).Build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem(m_device, m_renderer.GetSwapChainRenderPass(), globalSetLayout->GetDescriptorSetLayout());
		PointLightSystem pointLight(m_device, m_renderer.GetSwapChainRenderPass(), globalSetLayout->GetDescriptorSetLayout());

		Camera camera{};
		//camera.setViewTarget(glm::vec3(0.f,0.f,0.f),glm::vec3(0.0f,0.2f,1.f));
		camera.SetViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = GameObject::CreateGameObject();
		viewerObject.transform.translation.z = -2.5f;
		Keyboard cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_window.ShouldCloseWindow())
		{
			m_window.OnUpdate();

			ImGuiIO& io = ImGui::GetIO();
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.moveInPlaneXZ(m_window.getGLFWwindow(), frameTime, viewerObject);
			camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = m_renderer.GetAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
			camera.SetPerspectiveProjection(glm::radians(45.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = m_renderer.BeginFrame())
			{

				int frameIndex = m_renderer.GetFrameIndex();

				FrameInfo frameInfo = { frameIndex,frameTime,commandBuffer,camera,globalDescriptorSets[frameIndex],m_gameObjects };

				//update
				GlobalUBO ubo{};

				ubo.projection = camera.GetProjection();
				ubo.view = camera.GetView();

				pointLight.Update(frameInfo, ubo);

				uboBuffers[frameIndex]->WriteToBuffer(&ubo);
				uboBuffers[frameIndex]->Flush();
				// render
				m_renderer.BeginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.RenderGameObjects(frameInfo);
				pointLight.Render(frameInfo);

				m_ui.beginFrame();

				// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
				{
					static float f = 0.0f;
					static int counter = 0;

					ImGui::Begin("Merhaba Dünya!");                          // Create a window called "Hello, world!" and append into it.

					ImGui::Text("Merhaba Hocam.");        

					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
					ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

					if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
						counter++;
					ImGui::SameLine();
					ImGui::Text("counter = %d", counter);

					ImGui::Text("Renderer ortalama %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
					ImGui::End();
				}

				m_ui.endFrame(commandBuffer);

				m_renderer.EndSwapChainRenderPass(commandBuffer);
				m_renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_device.GetDevice());



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
		std::shared_ptr<Model> model = Model::CreateModelFormFile(m_device, "models/flat_vase.obj");

		auto flatVase = GameObject::CreateGameObject();
		flatVase.model = model;
		flatVase.transform.translation = { -0.5f,0.5f,0.f };
		flatVase.transform.scale = { 1.f,1.f,1.f };
		m_gameObjects.emplace(flatVase.getId(), std::move(flatVase));

		model = Model::CreateModelFormFile(m_device, "models/smooth_vase.obj");
		auto smoothVase = GameObject::CreateGameObject();
		smoothVase.model = model;
		smoothVase.transform.translation = { 0.5f,.5f,0.f };
		smoothVase.transform.scale = { 1.f,1.f,1.f };
		m_gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

		model = Model::CreateModelFormFile(m_device, "models/quad.obj");
		auto quad = GameObject::CreateGameObject();
		quad.model = model;
		quad.transform.translation = { 0.f,.5f,0.f };
		quad.transform.scale = { 3.f,1.f,3.f };
		m_gameObjects.emplace(quad.getId(), std::move(quad));

		model = Model::CreateModelFormFile(m_device, "models/cube.obj");
		auto cube = GameObject::CreateGameObject();
		cube.model = model;
		cube.transform.translation = { 0.2f,.2f,0.f };
		cube.transform.scale = { 0.1f,0.1f,0.1f };
		m_gameObjects.emplace(cube.getId(), std::move(cube));

		std::vector<glm::vec3> lightColors{
			  {1.f, .1f, .1f},
			  {.1f, .1f, 1.f},
			  {.1f, 1.f, .1f},
			  {1.f, 1.f, .1f},
			  {.1f, 1.f, 1.f},
			  {1.f, 1.f, 1.f}
		};

		for (int i = 0; i < lightColors.size(); i++) {
			auto pointLight = GameObject::MakePointLight(.1f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(
				glm::mat4(1.f),
				(i * glm::two_pi<float>()) / lightColors.size(),
				{ 0.f, -1.f, 0.f });
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
			m_gameObjects.emplace(pointLight.getId(), std::move(pointLight));
		}
	}

	/*void App::InitGUI()
	{
		std::unique_ptr<DescriptorPool> imguiPool;

		DescriptorPool::Builder builder(m_device);

		builder.SetMaxSets(1000 * 11);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER,1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000);
		builder.AddPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000);

		imguiPool = builder.Build();


		auto queueFamilyIndices = m_device.FindPhysicalQueueFamilies();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.Fonts->AddFontDefault();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
			// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(m_window.getGLFWwindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_device.GetInstance();
		init_info.PhysicalDevice = m_device.GetPhysicalDevice();
		init_info.Device = m_device.GetDevice();
		init_info.QueueFamily = queueFamilyIndices.graphicsFamily.value();
		init_info.Queue = m_device.GetGraphicsQueue();
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = imguiPool->GetDescriptorPool();
		//init_info.Subpass = 0;
		init_info.MinImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
		init_info.ImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = nullptr;
		init_info.CheckVkResultFn = nullptr;
		ImGui_ImplVulkan_Init(&init_info, m_renderer.GetSwapChainRenderPass());
		

		// Upload Fonts
		{
			VkCommandBuffer commandBuffer = m_device.BeginSingleTimeCommands();
			ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
			m_device.EndSingleTimeCommands(commandBuffer);
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}
	}
	*/
}