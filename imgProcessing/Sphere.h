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
		virtual Hit IntersectRayCollision(const Ray& ray) override {

			Hit hit{ glm::vec3(0.0f),glm::vec3(0.0f),-1.0f };

			// point = o + ud  o : ray.start / u : ray.dir  
			// ax^2+2bx+c = 0
			float b = glm::dot(ray.dir, ray.start - center);				// dot(u,o-c)
			float c = glm::dot(ray.start - center, ray.start - center) - r * r;	// (u-c)^2 - r^2

			float del = b * b - c;
			if (del >= 0) {
				float d1 = (-b + sqrt(b * b - c));
				float d2 = (-b - sqrt(b * b -  c));
				float d = glm::min(d1, d2);
				hit.d = d;
				hit.point = ray.start + ray.dir * hit.d;
				hit.normal = glm::normalize(hit.point - center);
			}
			return hit;

		}
	};
}