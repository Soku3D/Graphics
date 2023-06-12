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
    Hit IntersectRayCollision(const Ray& ray) override
    {
        Hit hit{-1.0f, glm::vec3(0.0f), glm::vec3(0.0f)};
        // ||s+du-c||^2 = r^2
        glm::vec3 cToS = ray.start - center;
        float b = glm::dot(ray.dir, cToS);
        float c = glm::dot(cToS, cToS) - (radius * radius);
        float nabla = (b * b) - c;
        if (nabla >= 0.0f)
        {
            float d1 = -b + glm::sqrt(nabla);
            float d2 = -b - glm::sqrt(nabla);
            hit.d = glm::min(d1, d2);
            hit.point = ray.start + ray.dir * hit.d;
            hit.norm = glm::normalize(hit.point - center);
        }
        return hit;
    }
};