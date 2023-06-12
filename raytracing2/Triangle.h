#pragma once
#include "Object.h"

class Triangle : public Object
{
public:
    glm::vec3 v0, v1, v2;
    glm::vec2 uv0, uv1, uv2;
    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
             glm::vec3 color = glm::vec3(0.0f))
        : v0(v0),
          v1(v1),
          v2(v2),
          Object(color)
    {
    }
    bool IntersectRayTriangle(glm::vec3 s, glm::vec3 dir, glm::vec3& n,
                              glm::vec3& point, glm::vec2& uv, float& d)
    {
        const glm::vec3 v01 = v1 - v0;
        const glm::vec3 v12 = v2 - v1;
        const glm::vec3 v20 = v0 - v2;

        glm::vec3 faceVec = glm::cross(v01, -v20);
        float faceAreaX2 = glm::length(faceVec);
        n = glm::normalize(faceVec);

        d = glm::dot(v0 - s, n) / glm::dot(dir, n);
        if (d < 0.0f)
            return false;
        if (glm::dot(-dir, n) < 1e-4)
            return false;

        point = s + d * dir;
        const glm::vec3 v0p = point - v0;
        const glm::vec3 v1p = point - v1;
        const glm::vec3 v2p = point - v2;

        const glm::vec3 cross0 = glm::cross(v01, v0p);
        const glm::vec3 cross1 = glm::cross(v12, v1p);
        const glm::vec3 cross2 = glm::cross(v20, v2p);

        const float area0 = glm::length(cross0) / faceAreaX2;
        const float area1 = glm::length(cross1) / faceAreaX2;
        const float area2 = glm::length(cross2) / faceAreaX2;

        uv = area0 * uv2 + area1 * uv0 + area2 * uv1;

        if (glm::dot(cross0, n) < 0.0f || glm::dot(cross1, n) < 0.0f ||
            glm::dot(cross2, n) < 0.0f)
            return false;

        return true;
    }
    virtual Hit IntersectRayCollision(const Ray& ray) override
    {
        Hit hit{-1.0f};
        float d;
        glm::vec3 n, point;
        glm::vec2 uv;
        if (IntersectRayTriangle(ray.start, ray.dir, n, point, uv, d))
        {
            hit.d = d;
            hit.point = point;
            hit.norm = n;
            hit.uv = uv;
        }
        return hit;
    }
};