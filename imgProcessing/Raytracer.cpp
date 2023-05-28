#include "Raytracer.h"

son::Raytracer::Raytracer(int width, int height)
	: width(width), height(height), imgWidth(width), imgHeight(height), imgChannels(3), aspect((float)width / height)
{
	auto sphere = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 1.0f), 0.7f);
	sphere->amb = glm::vec3(0.15f, 0.0f, 0.0f);
	sphere->diff = glm::vec3(1.0f, 0.1f, 0.1f);
	sphere->spec = glm::vec3(1.0f);
	sphere->alpha = 100.0f;
	sphere->ks = 5.0f;
	objects.push_back(sphere);

	auto square = std::make_shared<Square>(glm::vec3(-2.0f, -1.0f, 0.0f), glm::vec3(-2.0f, -1.0f, 4.0f), glm::vec3(2.0f, -1.0f, 4.0f), glm::vec3(2.0f, -1.0f, 0.0f));
	square->amb = glm::vec3(0.2f);
	square->diff = glm::vec3(0.5f);
	square->spec = glm::vec3(0.5f);
	square->alpha = 5.0f;
	square->ks = 5.0f;
	objects.push_back(square);
	light = Light{ {0.0f, 4.0f, 0.0f} };
}
son::Hit son::Raytracer::FindClosestObject(Ray& ray) {
	Hit closestHit{ glm::vec3(0.0f), glm::vec3(0.0f), -1.0f };
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
bool son::Raytracer::IsInShade(Ray& ray) {
	const Hit reflectHit = FindClosestObject(ray);
	if (reflectHit.d >= 0.0f)
	{
		return true;
	}
	return false;
}
glm::vec3 son::Raytracer::Raytrace(Ray& ray) {
	const Hit hit = FindClosestObject(ray);
	if (hit.d >= 0.0f)
	{
		glm::vec3 directToLight = glm::normalize(light.pos - hit.point);

		/*Ray reflectRay{ directToLight, hit.point + directToLight * 1e-2f };
		if (IsInShade(reflectRay))
			return hit.obj->amb;*/
		Ray lightRay;
		lightRay.dir = directToLight;
		lightRay.start = hit.point + hit.normal * 1e-2f;
		const auto hitLight = FindClosestObject(lightRay);
		if (hitLight.d >= 0.0f)
			return hit.obj->amb;
		float diff = glm::max(glm::dot(directToLight, hit.normal), 0.0f);

		glm::vec3 reflectDir = (2.0f * glm::dot(hit.normal, directToLight)) * hit.normal - directToLight;
		float spec = glm::pow(glm::max(glm::dot(reflectDir, -ray.dir), 0.0f), hit.obj->alpha);

		//return hit.obj->diff * diff;
		//return hit.obj->spec * spec * hit.obj->ks;
		return hit.obj->amb + hit.obj->diff * diff + hit.obj->spec * spec * hit.obj->ks;
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
			pixels[h * width + w] = glm::clamp(Raytrace(ray), glm::vec3(0.0f), glm::vec3(1.0f));
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
