#pragma once
#include "Object.h"

class Sphere : public Object
{
public:
    glm::vec3 center;
    float radius;
    Sphere(glm::vec3 center, float radius, glm::vec3 color = glm::vec3(0.0f))
        : center(center),
        radius(radius),
        Object(color)
    {
    }
    Hit IntersectRayCollision(Ray& ray) override {

    }
};