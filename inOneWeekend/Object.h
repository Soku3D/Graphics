#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Hit.h"
#include "Ray.h"
namespace son {
	using namespace std;
	using namespace glm;

	class Object 
	{
	public:
		vec3 amb;
		vec3 diff;
		vec3 spec;
		float alpha;

		Object(const vec3& color = { 1.0f, 1.0f, 1.0f })
			:amb(color), diff(color), spec(color),alpha(1.0f)
		{
		}

		virtual Hit CheckRayCollision(Ray& ray) = 0;
	};
}