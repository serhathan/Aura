#pragma once
#include "pch.h"
#include <Model.h>
#include "glm/gtc/matrix_transform.hpp"
namespace Aura {
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f,1.f,1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};
	class GameObject
	{
	public:
		using  id_t = unsigned int;

		static GameObject createGameObject() {
			static id_t current_id = 0;
			return GameObject(current_id++);
		}

		GameObject(const GameObject& c) = delete;
		GameObject& operator=(const GameObject& c) = delete;
		GameObject(GameObject&& c) = default;
		GameObject& operator=(GameObject&& c) = default;

		id_t getId() const { return id; }

		std::shared_ptr<Model> model{};
		glm::vec3 color;
		TransformComponent transform{};
	private:
		id_t id;
		GameObject(id_t id) : id(id) {};
	};
}
