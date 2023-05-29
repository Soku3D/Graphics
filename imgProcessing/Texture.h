#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
class Texture{
public:
	static std::string writePath;
	static std::string readPath;

	int width, height, channels;
	Texture(int width = 0, int height = 0, int channels = 0, std::string filename = "image");
	void ReadFromImage(std::vector<glm::vec3>& pixels, std::string filename = readPath);
	void WritePng(std::vector<glm::vec3>& pixels, std::string filename = writePath);
};