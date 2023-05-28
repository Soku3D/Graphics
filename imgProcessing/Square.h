#pragma once
#include "Object.h"
#include "Triangle.h"
namespace son {
	class Square : public Object {
	public:
		Triangle t1, t2;
		Square(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color = glm::vec3(1.0f))
			:t1(v0,v1,v2), t2(v0,v2,v3),Object(color)
		{}
		virtual Hit IntersectRayCollision(Ray& ray) override {
			Hit hit1 = t1.IntersectRayCollision(ray);
			Hit hit2 = t2.IntersectRayCollision(ray);

			if (hit1.d >= 0.0f)
				return hit1;
			else
				return hit2;
		}
	};
}