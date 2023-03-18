#pragma once
#include "pch.h"
#include "Device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "Buffer.h"

namespace Aura {
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
		glm::vec2 uv;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions() {
			std::vector<VkVertexInputBindingDescription> bindingDesc(1);
			bindingDesc[0].binding = 0;
			bindingDesc[0].stride = sizeof(Vertex);
			bindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDesc;
		}

		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
			std::vector<VkVertexInputAttributeDescription> attributeDesc{};

			attributeDesc.push_back({ 0,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex,position) });
			attributeDesc.push_back({ 1,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex,color) });
			attributeDesc.push_back({ 2,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex,normal) });
			attributeDesc.push_back({ 3,0,VK_FORMAT_R32G32_SFLOAT,offsetof(Vertex,uv) });

			return attributeDesc;
		}

		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && uv == other.uv;
		}
	};

	struct Builder
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		void loadModel(const std::string& filePath);
	};

	class Model
	{
	public:

		Model(Device& device, const Builder& builder);
		~Model();
		Model(const Model& c) = delete;
		Model& operator=(const Model& c) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

		static std::unique_ptr<Model> createModelFormFile(Device& device, const std::string& filePath);

	private:
		void createVertexBuffer(const std::vector<Vertex>& vertices);
		void createIndexBuffer(const std::vector<uint32_t>& indices);

		Device& device;

		std::unique_ptr<Buffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		std::unique_ptr<Buffer> indexBuffer;

		uint32_t indexCount;
	};
}
