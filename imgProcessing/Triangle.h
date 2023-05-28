#pragma once
#include "Object.h"
namespace son {
	class Triangle : public Object {
	public:
		glm::vec3 v0, v1, v2;
		Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color = glm::vec3(0.0f))
			:v0(v0), v1(v1), v2(v2), Object(color)
		{}
		virtual Hit IntersectRayCollision(Ray& ray) override {
			Hit hit{ glm::vec3(0.0f),glm::vec3(0.0f) ,-1.0f };
			glm::vec3 point, norm;
			float t, u, v;
			if (IntersectRayTriangle(ray.start, ray.dir, v0, v1, v2, norm, point, t, u, v))
			{
				hit.d = t;
				hit.normal = norm;
				hit.point = point;
			}
			return hit;
		}
		bool IntersectRayTriangle(glm::vec3 o, glm::vec3 d,
			glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
			glm::vec3& norm, glm::vec3& point, float& t, float& u, float& v)
		{
			glm::vec3 v01 = v1 - v0;
			glm::vec3 v12 = v2 - v1;
			glm::vec3 v20 = v0 - v2;

			norm = glm::normalize(glm::cross(v01, -v20));
			t = glm::dot((v0 - o), norm) / glm::dot(d, norm);
			if (t <= 0.0f)
				return false;

			if (glm::abs(glm::dot(norm, d)) <= 1e-3)
				return false;

			if (glm::dot(norm, -d) < 0.0f)
				return false;
			point = o + t * d;
			glm::vec3 v0p = point - v0;
			glm::vec3 v1p = point - v1;
			glm::vec3 v2p = point - v2;
			glm::vec3 cross1 = glm::cross(v01, v0p);
			glm::vec3 cross2 = glm::cross(v12, v1p);
			glm::vec3 cross3 = glm::cross(v20, v2p);

			if (glm::dot(cross1, norm) < 0.0f || glm::dot(cross2, norm) < 0.0f ||
				glm::dot(cross3, norm) < 0.0f)
				return false;

			return true;
		}
	};
}