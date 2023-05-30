#pragma once
#include "Object.h"
namespace son {

	class Triangle : public Object {
	public:
		glm::vec3 v0, v1, v2;
		glm::vec2 uv0, uv1, uv2;
		Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color = glm::vec3(0.0f))
			:v0(v0), v1(v1), v2(v2), Object(color)
		{}
		virtual Hit IntersectRayCollision(Ray& ray) override {
			Hit hit{ glm::vec3(0.0f),glm::vec3(0.0f) ,-1.0f };
			glm::vec3 point, norm;
			float t, w0, w1, w2;
			if (IntersectRayTriangle(ray.start, ray.dir, v0, v1, v2, norm, point, t, w0, w1, w2))
			{
				hit.d = t;
				hit.normal = norm;
				hit.point = point;
				hit.w0 = w0;
				hit.w1 = w1;
				hit.w2 = w2;
				hit.uv = uv0 * w0 + uv1 * w1 + uv2 * w2;
			}
			return hit;
		}
		bool IntersectRayTriangle(glm::vec3 o, glm::vec3 d,
			glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
			glm::vec3& norm, glm::vec3& point, float& t, float& w0, float& w1, float& w2)
		{
			glm::vec3 v01 = v1 - v0;
			glm::vec3 v12 = v2 - v1;
			glm::vec3 v20 = v0 - v2;

			norm = glm::normalize(glm::cross(v01, -v20));
			if (glm::dot(norm, -d) < 0.0f)
				return false;
			if (glm::abs(glm::dot(norm, d)) < 1e-2f)
				return false;

			t = glm::dot((v0 - o), norm) / glm::dot(d, norm);
			if (t < 0.0f)
				return false;

			point = o + t * d;
			glm::vec3 v0p = point - v0;
			glm::vec3 v1p = point - v1;
			glm::vec3 v2p = point - v2;
			glm::vec3 cross1 = glm::cross(v01, v0p);
			glm::vec3 cross2 = glm::cross(v12, v1p);
			glm::vec3 cross3 = glm::cross(v20, v2p);

			float faceArea = glm::length(glm::cross(v01, -v20)) / 2.0f;
			w0 = (glm::length(cross2) / 2.0f) / faceArea;
			w1 = (glm::length(cross3) / 2.0f) / faceArea;
			w2 = 1.0f - w0 - w1;

			if (glm::dot(cross1, norm) < 0.0f || glm::dot(cross2, norm) < 0.0f ||
				glm::dot(cross3, norm) < 0.0f)
				return false;

			return true;
		}
	};
}