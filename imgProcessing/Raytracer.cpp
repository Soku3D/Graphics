#include "Raytracer.h"


son::Raytracer::Raytracer(int width, int height)
	: width(width), height(height), imgWidth(width), imgHeight(height), imgChannels(3), aspect((float)width / height)
{
	auto sphere = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 2.0f), 0.4f);
	sphere->amb = glm::vec3(0.0f, 0.8f, 0.0f);
	sphere->diff = glm::vec3(0.0f, 0.8f, 0.0f);
	sphere->spec = glm::vec3(0.0f, 0.8f, 0.0f);
	objects.push_back(sphere);
	light.pos = glm::vec3(0.0f, 1.0f, -2.0f);
}
son::Hit son::Raytracer::FindClosestObject(Ray& ray) {
	Hit closestHit{ glm::vec3(0.0f), glm::vec3(0.0f), -1 };
	float closestD = 1e10;
	for (int i = 0; i < objects.size(); i++)
	{
		Hit hit = objects[i]->IntersectRayCollision(ray);
		if (hit.d >= 0.0f && hit.d <= closestD) {
			closestD = hit.d;
			closestHit = hit;
			closestHit.obj = objects[i];
		}
	}
	return closestHit;
}
glm::vec3 son::Raytracer::Raytrace(Ray& ray) {
	const Hit hit = FindClosestObject(ray);
	if (hit.d >= 0.0f)
	{
		return glm::vec3(1.0f);
	}
	return glm::vec3(0.0f);
}
void son::Raytracer::Render(std::vector<glm::vec3>& pixels) {
	std::fill(pixels.begin(), pixels.end(), glm::vec3(0.0f));
	glm::vec3 eyePos(0.0f, 0.0f, -2.0f);
#pragma omp parallel for
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			glm::vec3 WorldPos = TransformScreenToWorld(glm::vec3{ w, h, 0.0f });
			Ray ray{ glm::normalize(WorldPos - eyePos), WorldPos };
			pixels[h * width + w] = Raytrace(ray);
		}
	}
}
glm::vec3 son::Raytracer::TransformScreenToWorld(glm::vec3 screenPos) {
	float xScale = 2.0f / width;
	float yScale = 2.0f / height;
	screenPos.x *= xScale;
	screenPos.y *= yScale;
	return glm::vec3{ (screenPos.x - 1.0f) * aspect, -(screenPos.y - 1.0f) , 0.0f };
}
