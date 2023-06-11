#pragma once
#include <glm/glm.hpp>
#include <memory>

class Object;
class Hit
{
public:
    float d;
    glm::vec3 point;
    glm::vec3 norm;
    std::shared_ptr<Object> obj;
};