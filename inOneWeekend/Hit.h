#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace son {
	using namespace glm;

	class Hit {
	public:
		vec3 normal;
		vec3 hitPoint;
		float distance;
		std::shared_ptr<class Object> hitedObj;
	};
}