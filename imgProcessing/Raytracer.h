#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Hit.h"
#include "Object.h"
#include "Sphere.h"
#include "Light.h"
namespace son {
	class Raytracer {
	public:
		int width, height;
		float aspect;
		int imgWidth, imgHeight, imgChannels;
		
		Light light;
		std::vector < std::shared_ptr<Object>> objects;
		Raytracer(int width, int height);
		void Render(std::vector<glm::vec3>& pixels);
		glm::vec3 TransformScreenToWorld(glm::vec3 screenPos);
		glm::vec3 Raytrace(Ray& ray);
		Hit FindClosestObject(Ray& ray);
	};

}