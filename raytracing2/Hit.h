#pragma once
#include <memory>
#include <glm/glm.hpp>

class Object;
class Hit {
	float d;
	glm::vec3 point;
	glm::vec3 norm;
	std::shared_ptr<Object> obj;
};