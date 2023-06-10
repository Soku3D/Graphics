#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
class Texture {
public:
	std::vector<unsigned char> mPixels;
	int width, height, channels;
	std::string texturePath;
	std::string resultPath;

	Texture(const std::string filename = "test");
	Texture(int width, int height, int channels = 3, const std::vector<glm::vec3>& pixels);

	void RenderImage(const std::string filename = "1");
};