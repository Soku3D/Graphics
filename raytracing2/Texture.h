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

    void RenderImage(const std::string filename = "1",
                     const std::string saveDir = "");
    glm::vec3 GetClamped(int i, int j)
    {

        i = glm::clamp(i, 0, width - 1);
        j = glm::clamp(j, 0, height - 1);
        int idx = (i * width + j) * channels;
        float r = mPixels[idx] / 255.0f;
        float g = mPixels[idx + 1] / 255.0f;
        float b = mPixels[idx + 2] / 255.0f;
        return glm::vec3(r, g, b);
    }
    glm::vec3 GetWrapped(int i, int j)
    {
        i += width;
        j += height;

        i %= width;
        j %= height;

        int idx = (i * width + j) * channels;
        float r = mPixels[idx] / 255.0f;
        float g = mPixels[idx + 1] / 255.0f;
        float b = mPixels[idx + 2] / 255.0f;
        return glm::vec3(r, g, b);
    }
    glm::vec2 TransformUVToImage(const glm::vec2& uv)
    {
        float x = uv.x * width - 0.5f;
        float y = uv.y * height - 0.5f;
        int i = (int)glm::round(x);
        int j = (int)glm::round(y);

        return glm::vec2(i, j);
    }
    glm::vec3 GetTexture(glm::vec2& uv)
    {
        glm::vec2 imagePos = TransformUVToImage(uv);
        return GetClamped(imagePos.x, imagePos.y);
    }
    glm::vec3 GetLinear(glm::vec2& uv)
    {
        glm::vec2 imagePos = TransformUVToImage(uv);
        return GetClamped(imagePos.x, imagePos.y);
    }
};