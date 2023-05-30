#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Hit.h"
#include "Object.h"
#include "Sphere.h"
#include "Light.h"
#include "Triangle.h"
#include "Square.h"
#include "Texture.h"
namespace son {
	class Raytracer {
	public:
		glm::vec3 eyePos;
		int width, height;
		float aspect;
		int imgWidth, imgHeight, imgChannels;
		Raytracer(int width, int height);

		Light light;
		std::vector < std::shared_ptr<Object>> objects;
	public:
		void Render(std::vector<glm::vec3>& pixels);
		glm::vec3 TransformScreenToWorld(glm::vec3 screenPos);
		glm::vec3 TraceRay(const Ray& ray);
		glm::vec3 TraceRay2x2(glm::vec3& worldPos, const float& dx, int samplingCount = 1);
		Hit FindClosestObject(const Ray& ray);
		bool IsInShade(const Ray& ray);
	};

}