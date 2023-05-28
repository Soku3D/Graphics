#pragma once
#include "Object.h"
namespace son {
	using namespace glm;

	class Sphere : public Object {
	public: 
		vec3 center;
		float radius;

		Sphere(vec3 center, float radius, vec3 color)
			:center(center),
			radius(radius),
			Object(color)
		{

		}
		~Sphere()
		{
		}

		Hit CheckRayCollision(Ray& ray) {
			Hit hit{vec3(0.0f),vec3(0.0f),-1.0f};
			vec3 rayToCenter = center - ray.startPoint;
			vec3 innerToDir = ray.dir * glm::dot(ray.dir, rayToCenter);
			vec3 centerToDir = innerToDir - rayToCenter;
			// if ray Hit the sphere
			if (glm::dot(centerToDir, centerToDir) < radius * radius) {
				hit.distance = glm::length(innerToDir) - glm::sqrt(radius * radius - glm::dot(centerToDir, centerToDir));
				hit.hitPoint = ray.dir * hit.distance;
				hit.normal = glm::normalize(hit.hitPoint - center);
			}
			return hit;
		}
	};
}