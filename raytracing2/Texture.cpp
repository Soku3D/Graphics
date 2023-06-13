#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "Texture.h"

Texture::Texture(const std::string filename)
    : texturePath("Textures\\"),
      resultPath("Results\\"),
      width(0),
      height(0),
      channels(0)
{
    std::string currPath = texturePath + filename + ".png";

    stbi_uc* img = stbi_load(currPath.c_str(), &width, &height, &channels, 0);
    mPixels.resize(width * height * channels);
    memcpy(mPixels.data(), img, mPixels.size() * sizeof(uint8_t));

    delete img;
}
Texture::Texture(int width, int height, int channels,
                 const std::vector<glm::vec3>& pixels)
    : texturePath("Textures\\"),
      resultPath("Results\\"),
      width(width),
      height(height),
      channels(channels)
{
    mPixels.resize(width * height * channels);
    for (int idx = 0; idx < pixels.size(); idx++)
    {
        mPixels[idx * channels] = static_cast<stbi_uc>(pixels[idx].x * 255.0);
        mPixels[idx * channels + 1] =
            static_cast<stbi_uc>(pixels[idx].y * 255.0);
        mPixels[idx * channels + 2] =
            static_cast<stbi_uc>(pixels[idx].z * 255.0);
    }
}
void Texture::RenderImage(const std::string filename, const std::string saveDir)
{
    resultPath += saveDir + "\\";
    std::string currPath = resultPath + filename + ".png";
    stbi_write_png(currPath.c_str(), width, height, channels, mPixels.data(),
                   width * channels);
}
