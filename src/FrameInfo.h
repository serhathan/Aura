#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Camera.h"

namespace Aura {

#define MAX_LIGHTS 10

	struct PointLight
	{
		glm::vec4 position{};
		glm::vec4 color{};
	};

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
		GameObject::Map& gameObjects;
	};

	struct GlobalUBO
	{
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::vec4 ambientLightColor{ 1.f,1.f,1.f,0.02f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;

	};
}