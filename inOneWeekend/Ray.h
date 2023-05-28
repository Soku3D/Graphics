#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
namespace son {
	using namespace glm;

	class Ray {
	public:
		vec3 dir;
		vec3 startPoint;
	};
}