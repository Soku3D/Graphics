#pragma once
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
#include "Light.h"
#include "Triangle.h"
#include "Rectangle.h"
class Raytracer
{
public:
    int width, height;
    float aspect;
    glm::vec3 eyePos;
    Raytracer(int width, int height);
public:
    std::vector<std::shared_ptr<Object>> objects;
    Light light;
public:
    Hit FindClosestCollision(const Ray& ray);
    glm::vec3 traceRay2x2(glm::vec3& currPos, float currD, int count);
    glm::vec3 traceRay(const Ray& ray);
    void Render(std::vector<glm::vec3>& pixels);
    glm::vec3 TransformScreenToWorld(const glm::vec2& pos);

    bool IsShadow(const Ray& ray);
};