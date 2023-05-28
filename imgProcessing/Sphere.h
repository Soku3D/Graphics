#pragma once

#include "Object.h"
namespace son {
	class Sphere : public Object {
	public:
		glm::vec3 center;
		float r;
		Sphere(glm::vec3 center, float r, glm::vec3 color = glm::vec3(0.0f))
			:center(center), r(r), Object(color)
		{
		}
		virtual Hit IntersectRayCollision(Ray& ray) override {

			Hit hit{ glm::vec3(0.0f),glm::vec3(0.0f),-1.0f };
			glm::vec3 rayToCenter = center - ray.start;
			glm::vec3 innerToDir = ray.dir * glm::dot(ray.dir, rayToCenter);
			if (glm::dot(ray.dir, rayToCenter) < 0.0f)
				return hit;
			glm::vec3 centerToDir = innerToDir - rayToCenter;
			if (r * r >= glm::dot(centerToDir, centerToDir)) {
				hit.d = glm::length(innerToDir) - glm::sqrt(r*r - glm::dot(centerToDir, centerToDir));
				hit.point = ray.start + ray.dir * hit.d;
				hit.normal = glm::normalize(hit.point - center);
			}
			return hit;

		}
	};
}