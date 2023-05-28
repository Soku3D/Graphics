#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "Ray.h"

namespace son {
	class Hit {
	public:
		glm::vec3 point;
		glm::vec3 normal;
		float d;
		std::shared_ptr<class Object> obj;
	};
}