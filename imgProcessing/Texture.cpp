
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

std::string Texture::writePath = "";
std::string Texture::readPath = "";
Texture::Texture(int width , int height, int channels , std::string filename)
	:width(width), height(height), channels(channels)
{
	char* pPath = getenv("GRAPHICS_PATH");
	writePath = pPath;
	writePath += "/";
	writePath += filename;
	writePath += ".png";
	readPath = "textures/texturetest.png";
	
}
void Texture::ReadFromImage(std::vector<glm::vec3>& pixels, std::string filename) {

	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	pixels.resize(width * height);
	for (int i = 0; i < width * height; i++)
	{
		pixels[i] = glm::vec3(img[i * channels] / 255.0f, img[i * channels + 1] / 255.0f,
			img[i * channels + 2] / 255.0f);
	}
	delete[] img;
}
void Texture::WritePng(std::vector<glm::vec3>& pixels, std::string filename) {
	std::vector<unsigned char> img(width * height * channels, 0);
	for (int i = 0; i < width * height; i++)
	{
		img[i * channels] = uint8_t(pixels[i].x * 255.0f);
		img[i * channels + 1] = uint8_t(pixels[i].y * 255.0f);
		img[i * channels + 2] = uint8_t(pixels[i].z * 255.0f);
	}
	stbi_write_png(filename.c_str(), width, height, channels, img.data(), width * channels);
	//stbi_write_jpg(writePath.c_str(), width, height, channels, img.data(), 100);
	system(filename.c_str());
}