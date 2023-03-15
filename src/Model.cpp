#include "Model.h"

namespace Aura {
	Model::Model(Device& device, const Builder& builder) : device(device)
	{
		createVertexBuffer(builder.vertices);
		createIndexBuffer(builder.indices);
	}
	Model::~Model()
	{
		vkDestroyBuffer(device.device(),vertexBuffer,nullptr);
		vkFreeMemory(device.device(),vertexBufferMemory,nullptr);

		if (hasIndexBuffer)
		{
			vkDestroyBuffer(device.device(), indexBuffer, nullptr);
			vkFreeMemory(device.device(), indexBufferMemory, nullptr);
		}
	}
	void Model::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] ={vertexBuffer};

		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer,0,1,buffers,offsets);
		if (hasIndexBuffer)
			vkCmdBindIndexBuffer(commandBuffer,indexBuffer,0,VK_INDEX_TYPE_UINT32);

	}
	void Model::draw(VkCommandBuffer commandBuffer)
	{
		if (hasIndexBuffer)
		{
			vkCmdDrawIndexed(commandBuffer,indexCount,1,0,0,0);
		}
		else
		{
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		}
	}
	void Model::createVertexBuffer(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >=3 && "Vertex Count Must Be At Least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		VkBuffer stageBuffer;
		VkDeviceMemory stageBufferMemory;

		device.createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stageBuffer, stageBufferMemory);

		void* data;
		vkMapMemory(device.device(), stageBufferMemory,0,bufferSize,0,&data);
		memcpy(data,vertices.data(),static_cast<size_t>(bufferSize));
		vkUnmapMemory(device.device(), stageBufferMemory);

		device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

		device.copyBuffer(stageBuffer,vertexBuffer,bufferSize);

		vkDestroyBuffer(device.device(), stageBuffer, nullptr);
		vkFreeMemory(device.device(), stageBufferMemory, nullptr);


	}
	void Model::createIndexBuffer(const std::vector<uint32_t>& indices)
	{
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = indexCount > 0;

		if (!hasIndexBuffer)
			return;

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

		VkBuffer stageBuffer;
		VkDeviceMemory stageBufferMemory;

		device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stageBuffer, stageBufferMemory);

		void* data;
		vkMapMemory(device.device(), stageBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(device.device(), stageBufferMemory);

		device.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		device.copyBuffer(stageBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(device.device(), stageBuffer, nullptr);
		vkFreeMemory(device.device(), stageBufferMemory, nullptr);
	}
}