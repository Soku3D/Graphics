#pragma once

#include "Hit.h"

namespace son {
	class Object {
	public:
		glm::vec3 amb;
		glm::vec3 diff;
		glm::vec3 spec;
		float ks;
		float alpha;
		Object(glm::vec3 color = glm::vec3(0.0f))
			:amb(color), diff(color), spec(color), ks(0.8f), alpha(0.8f)
		{}
		virtual Hit IntersectRayCollision(Ray& ray) = 0;
	};
}