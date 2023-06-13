#pragma once
#include "Object.h"
#include "Triangle.h"
class Rectangle : public Object
{
public:
    Triangle tri0, tri1;
    Rectangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
              float uvSize = 1.0f, glm::vec3 color = glm::vec3(0.0f))
        : tri0(v0, v1, v2),
          tri1(v0, v2, v3),
          Object(color)
    {
        tri0.uv0 = glm::vec2(0.0f, uvSize);
        tri0.uv1 = glm::vec2(0.0f, 0.0f);
        tri0.uv2 = glm::vec2(uvSize, 0.0f);

        tri1.uv0 = glm::vec2(0.0f, uvSize);
        tri1.uv1 = glm::vec2(uvSize, 0.0f);
        tri1.uv2 = glm::vec2(uvSize, uvSize);
    }

    virtual Hit IntersectRayCollision(const Ray& ray) override
    {
        Hit hit0 = tri0.IntersectRayCollision(ray);
        Hit hit1 = tri1.IntersectRayCollision(ray);

        if (hit0.d >= 0.0f)
            return hit0;
        else
        {
            return hit1;
        }
    }
};