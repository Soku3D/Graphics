#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
class Texture{
public:
	std::string writePath;
	int width, height, channels;
	Texture(int width = 0, int height = 0, int channels = 0, std::string filename = "image");
	void ReadFromImage(const char* filename, std::vector<glm::vec3>& pixels);
	void WritePng(std::vector<glm::vec3>& pixels);
};