#include "Hit.h"
#include "Sphere.h"
#include <vector>
namespace son {
	using namespace std;
	using namespace glm;

	class Raytracer {
	public:
		int width, height;
		float aspect;
		vector<shared_ptr<Object>> objects;
		Raytracer(int width, int height)
			:width(width), height(height), aspect((float)width / height)
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.0f, 0.0f, 1.0f), 0.4, vec3(0.0f));
			sphere1->diff = vec3(1.0f, 0.0f, 0.0f);
			sphere1->amb = vec3(1.0f, 0.0f, 0.0f);
			objects.push_back(sphere1);
		}
		Hit ClosestHitPos(Ray& ray) {
			Hit hit{ vec3(0.0f),vec3(0.0f),-1.0f };
			double closestDistance = 1e10;
			size_t objectSZ = objects.size();
			for (size_t i = 0; i < objectSZ; i++)
			{
				Hit checkHited = objects[i]->CheckRayCollision(ray);
				if (checkHited.distance > 0) {
					if (closestDistance > checkHited.distance)
					{
						closestDistance = checkHited.distance;
						hit = checkHited;
						hit.hitedObj = objects[i];
					}
				}
			}
			return hit;
		}
		vec3 Raytrace(Ray& ray) {
			Hit hit = ClosestHitPos(ray);
			if (hit.distance > 0) 
			{
				return hit.hitedObj->amb;
			}
			else {
				return vec3(0.0f);
			}
		}
		void Render(vector<glm::vec3>& pixels) {
			vec3 eyePos{ 0.0f, 0.0f, -2.0f };
			fill(pixels.begin(), pixels.end(), vec3(0.0f, 0.0f, 0.0f));
#pragma omp parallel for
			for (int yPos = 0; yPos < height; yPos++)
			{
				for (int xPos = 0; xPos < width; xPos++)
				{
					vec2 worldPos = TransformScreenToWorld(vec2{ xPos, yPos });
					Ray ray;
					ray.startPoint = vec3(worldPos, 0.0f);
					ray.dir = glm::normalize(ray.startPoint - eyePos);
					pixels[width * yPos + xPos] = Raytrace(ray);
				}
			}
		}
		vec2 TransformScreenToWorld(vec2 vec) {
			float worldXPos = (vec.x * 2.0 / width) - 1;
			float worldYPos = (vec.y * 2.0 / height) - 1;
			return vec2{ worldXPos * aspect, -worldYPos };
		}
	};

}