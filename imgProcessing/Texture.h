#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
class Texture{
public:
	static std::string writePath;
	static std::string readPath;
	std::vector<uint8_t> image;
	int width, height, channels;
	Texture();
	Texture(int width, int height, int channels, std::vector<glm::vec3>& pixels);

	void ReadFromImage(std::string filename = readPath);
	void WritePng(std::string filename);
	glm::vec3 GetClamped(int i, int j);
	glm::vec3 GetWrapped(int i, int j);
	glm::vec3 InterpolateBilinear(const float& dx, const float& dy,
	const glm::vec3 c00, const glm::vec3 c10, const glm::vec3 c01, const glm::vec3 c11);
	glm::vec3 SamplePoint(const glm::vec2& uv , const char* str ="clamped");
	glm::vec3 SampleLinear(const glm::vec2& uv);
	glm::vec2 TranseformUvToImage(const glm::vec2& uv);
};