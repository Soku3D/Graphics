#pragma once
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
class Raytracer
{
public:
    int width, height;
    Raytracer(int width, int height);
    void Render(std::vector<glm::vec3> pixels);
};