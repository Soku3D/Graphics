
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

std::string Texture::writePath = "";
std::string Texture::readPath = "";
Texture::Texture()
{
	char* pPath = getenv("GRAPHICS_PATH");
	writePath = pPath;
	readPath = "textures/texturetest.png";
	unsigned char* img = stbi_load(readPath.c_str(), &width, &height, &channels, 0);
	image.resize(width * height * channels);
	memcpy(image.data(), img, image.size() * sizeof(uint8_t));
	delete img;
}
Texture::Texture(int width, int height, int channels, std::vector<glm::vec3>& pixels)
	:width(width), height(height), channels(channels)
{
	char* pPath = getenv("GRAPHICS_PATH");
	writePath = pPath;
	image.resize(width * height * channels);
	readPath = "textures/texturetest.png";
	for (int i = 0; i < width * height; i++)
	{
		image[i * channels] = (uint8_t)(pixels[i].x * 255.0f);
		image[i * channels + 1] = (uint8_t)(pixels[i].y * 255.0f);
		image[i * channels + 2] = (uint8_t)(pixels[i].z * 255.0f);
	}
}

void Texture::ReadFromImage(std::string filename) {

	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	image.resize(width * height * channels);
	memcpy(image.data(), img, image.size() * sizeof(uint8_t));

	delete img;
}
void Texture::WritePng(std::string filename) {
	writePath += "/";
	writePath += filename;
	writePath += ".png";
	stbi_write_png(writePath.c_str(), width, height, channels, image.data(), width * channels);
	//stbi_write_jpg(writePath.c_str(), width, height, channels, img.data(), 100);
	system(writePath.c_str());
}
glm::vec3 Texture::GetClamped(int i, int j) {
	i = glm::clamp(i, 0, width - 1);
	j = glm::clamp(j, 0, height - 1);

	int idx = i * channels + j * channels * width;
	const float r = image[idx] / 255.0f;
	const float g = image[idx + 1] / 255.0f;
	const float b = image[idx + 2] / 255.0f;

	return glm::vec3(r, g, b);
}
glm::vec3 Texture::GetWrapped(int i, int j) {
	i %= width;
	j %= height;
	if (i < 0) i += width;
	if (j < 0) j += height;

	int idx = (i + width * j) * channels;
	const float r = image[idx] / 255.0f;
	const float g = image[idx + 1] / 255.0f;
	const float b = image[idx + 2] / 255.0f;

	return glm::vec3(r, g, b);
}
glm::vec2 Texture::TranseformUvToImage(const glm::vec2& uv) {
	return (uv * glm::vec2(width, height) - glm::vec2(0.5f));
}
glm::vec3 Texture::SamplePoint(const glm::vec2& uv, const char* str) {
	// image coordinate  [-0.5, width-0.5] * [-0.5, height-0.5]
	glm::vec2 imagePos = TranseformUvToImage(uv);
	int x = glm::round(imagePos.x);
	int y = glm::round(imagePos.y);
	if (str == "wrapped") {
		return GetWrapped(x, y);
	}
	else if (str == "clamped") {
		
		return GetClamped(x, y);
	}
	else
		return glm::vec3(0.0f);
	
}
glm::vec3 Texture::InterpolateBilinear(const float& dx,const float& dy,
	const glm::vec3 c00, 
	const glm::vec3 c10,
	const glm::vec3 c01, 
	const glm::vec3 c11 ) {
	
	const glm::vec3 interpolateX1 = c00 * (1.0f - dx) + c10 * dx;
	const glm::vec3 interpolateX2 = c01 * (1.0f - dx) + c11 * dx;
	return interpolateX1 * (1.0f - dy) + interpolateX2 * dy;
}
glm::vec3 Texture::SampleLinear(const glm::vec2& uv) {
	glm::vec2 imagePos = TranseformUvToImage(uv);
	int x = glm::floor(imagePos.x);
	int y = glm::floor(imagePos.y);
	glm::vec3 c00 = GetWrapped(x, y);
	glm::vec3 c10 = GetWrapped(x+1, y);
	glm::vec3 c01 = GetWrapped(x, y+1);
	glm::vec3 c11 = GetWrapped(x+1, y+1);

	float dx = imagePos.x - (float)x;
	float dy = imagePos.y - (float)y;

	return InterpolateBilinear(dx,dy,c00,c10,c01,c11);
}
