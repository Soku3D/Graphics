#include "Raytracer.h"

son::Raytracer::Raytracer(int width, int height)
	: width(width), height(height), imgWidth(width), imgHeight(height), imgChannels(3), aspect((float)width / height)
{
	eyePos = glm::vec3(0.0f, 0.0f, -1.5f);
	auto sphere = std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, 1.5f), 0.8f);
	//auto sphere = std::make_shared<Sphere>(glm::vec3(1.0f, 1.0f, 2.0f), 0.7f);
	sphere->amb = glm::vec3(0.15f, 0.0f, 0.0f);
	sphere->diff = glm::vec3(1.0f, 0.1f, 0.1f);
	sphere->spec = glm::vec3(1.0f);
	sphere->alpha = 100.0f;
	sphere->ks = 5.0f;
	objects.push_back(sphere);

	auto square = std::make_shared<Square>(glm::vec3(-2.0f, 2.0f, 3.0f), glm::vec3(2.0f, 2.0f, 3.0f), glm::vec3(2.0f, -2.0f, 3.0f), glm::vec3(-2.0f, -2.0f, 3.0f),
		glm::vec2(0.0f), glm::vec2(4.0f, 0.0f), glm::vec2(4.0f), glm::vec2(0.0f, 4.0f));
	square->amb = glm::vec3(0.2f);
	square->diff = glm::vec3(0.5f);
	square->spec = glm::vec3(0.5f);
	square->alpha = 5.0f;
	square->ks = 5.0f;

	//auto texture = std::make_shared<Texture>();
	//square->ambTexture = texture;
	objects.push_back(square);

	/*auto triangle = std::make_shared<Triangle>(glm::vec3(-2.0f, -2.0f, 2.0f), glm::vec3(-2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	triangle->amb = glm::vec3(0.2f);
	triangle->diff = glm::vec3(0.5f);
	triangle->spec = glm::vec3(0.5f);
	triangle->alpha = 5.0f;
	triangle->ks = 5.0f;*/
	//objects.push_back(triangle);

	light = Light{ {0.0f, 1.0f, 0.5f} };
}
son::Hit son::Raytracer::FindClosestObject(const Ray& ray) {
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
bool son::Raytracer::IsInShade(const Ray& ray) {
	const Hit reflectHit = FindClosestObject(ray);
	if (reflectHit.d >= 0.0f)
	{
		return true;
	}
	return false;
}
glm::vec3 son::Raytracer::TraceRay(const Ray& ray) {
	const Hit hit = FindClosestObject(ray);
	if (hit.d >= 0.0f)
	{
		glm::vec3 phongColor(0.0f);

		/*glm::vec3 v0Color(1.0f, 0.0f, 0.0f);
		glm::vec3 v1Color(0.0f, 1.0f, 0.0f);
		glm::vec3 v2Color(0.0f, 0.0f, 1.0f);*/

		if (hit.obj->ambTexture != nullptr) {
			phongColor += hit.obj->ambTexture->SamplePoint(hit.uv, "clamped");
		}
		else {
			phongColor += hit.obj->amb;
		}
		//return hit.w0 * v0Color + hit.w1 * v1Color + hit.w2 * v2Color;

		glm::vec3 directToLight = glm::normalize(light.pos - hit.point);

		//Ray reflectRay{ directToLight, hit.point + directToLight * 1e-2f };
		//if (IsInShade(reflectRay))
			//return hit.obj->amb;

		float diff = glm::max(glm::dot(directToLight, hit.normal), 0.0f);

		glm::vec3 reflectDir = (2.0f * glm::dot(hit.normal, directToLight)) * hit.normal - directToLight;
		float spec = glm::pow(glm::max(glm::dot(reflectDir, -ray.dir), 0.0f), hit.obj->alpha);

		//return hit.obj->diff * diff;
		//return hit.obj->spec * spec * hit.obj->ks;
		return  phongColor + hit.obj->diff * diff + hit.obj->spec * spec * hit.obj->ks;
	}
	return glm::vec3(0.0f);
}
glm::vec3 son::Raytracer::TraceRay2x2(glm::vec3& pixelPos, const float& dx, int samplingCount) {
	if (samplingCount == 0)
	{
		Ray myRay{ glm::normalize(pixelPos - eyePos), pixelPos  };
		return TraceRay(myRay);
	}
	float subDx = dx * 0.5f;
	glm::vec3 color(0.0f);
	glm::vec3 p00 = pixelPos - glm::vec3(subDx * 0.5f, subDx * 0.5f, 0.0f);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			glm::vec3 subPos = p00 + glm::vec3(subDx * float(i), subDx * float(j), 0.0f);
			color += TraceRay2x2(subPos, subDx, samplingCount - 1);
		}
	}
	return color * 0.25f;
}
void son::Raytracer::Render(std::vector<glm::vec3>& pixels) {
	std::fill(pixels.begin(), pixels.end(), glm::vec3(0.0f));

#pragma omp parallel for
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			glm::vec3 WorldPos = TransformScreenToWorld(glm::vec3{ w, h, 0.0f });
			Ray ray{ glm::normalize(WorldPos - eyePos), WorldPos };
			float dx = 2.0f / height;
			const auto pixelColor = TraceRay2x2(WorldPos, dx, 0);
			pixels[h * width + w] = glm::clamp(pixelColor, 0.0f, 1.0f);
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
