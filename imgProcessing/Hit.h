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
		float w0, w1, w2;
		glm::vec2 uv;
		std::shared_ptr<class Object> obj;
	};
}