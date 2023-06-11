#pragma once
#include <glm/glm.hpp>
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"
class Object
{
public:
    glm::vec3 amb;
    glm::vec3 dif;
    glm::vec3 spec;
    std::shared_ptr<Texture> texture;

    Object(glm::vec3 color)
        : amb(color),
          dif(color),
          spec(color),
          texture(nullptr)
    {
    }
    virtual Hit IntersectRayCollision(Ray& ray) = 0;
};