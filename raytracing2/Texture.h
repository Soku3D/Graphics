#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
class Texture
{
public:
    std::vector<unsigned char> mPixels;
    int width, height, channels;
    std::string texturePath;
    std::string resultPath;
    Texture(const std::string filename = "test");
    Texture(int width, int height, int channels,
            const std::vector<glm::vec3>& pixels);

    void RenderImage(const std::string filename = "1");
};