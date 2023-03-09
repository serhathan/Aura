#pragma once
#include "pch.h"
#include <Model.h>
namespace Aura {

	struct Transform2DComponent
	{
		glm::vec2 translation {};
		glm::vec2 scale {1.f,1.f};
		float rotation;
		glm::mat2 mat2()
		{
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);

			glm::mat2 rotationMat = {{c,s},{-s,c}};
			glm::mat2 scaleMat {{scale.x,.0f},{.0f,scale.y}};

			return rotationMat * scaleMat;
		}
	};
	class GameObject
	{
	public:
		using  id_t = unsigned int;

		static GameObject createGameObject() {
			static id_t current_id = 0;
			return GameObject(current_id++);
		}


		GameObject(const GameObject& c ) = delete;
		GameObject& operator=(const GameObject& c) = delete;
		GameObject(GameObject&& c) = default;
		GameObject& operator=(GameObject&& c) = default;

		
		id_t getId() const {return id;}

		std::shared_ptr<Model> model {};
		glm::vec3 color;
		Transform2DComponent transform2D{};
	private:
		id_t id;
		GameObject(id_t id) : id(id){};

	};
}



