#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

using pii = std::pair<int,int>;
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

        int idx = (j * width + i) * channels;

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

        int idx = (j * width + i) * channels;

        float r = mPixels[idx] / 255.0f;
        float g = mPixels[idx + 1] / 255.0f;
        float b = mPixels[idx + 2] / 255.0f;
        return glm::vec3(r, g, b);
    }
    glm::vec2 TransformUVToImageR(const glm::vec2& uv)
    {
        float x = uv.x * width - 0.5f;
        float y = uv.y * height - 0.5f;
        int i = (int)glm::round(x);
        int j = (int)glm::round(y);

        return glm::vec2(i, j);
    }
    pii TransformUVToImageF(const glm::vec2& uv)
    {
        float x = uv.x * width - 0.5f;
        float y = uv.y * height - 0.5f;
        int i = (int)glm::floor(x);
        int j = (int)glm::floor(y);

        return {i, j};
    }
    glm::vec3 GetTexture(glm::vec2& uv)
    {
        glm::vec2 imagePos = TransformUVToImageR(uv);
        return GetClamped(imagePos.x, imagePos.y);
    }
    glm::vec3 GetLinear(glm::vec2& uv)
    {
        // transform uv coordinate to image coordinate
        float x = uv.x * width - 0.5f; 
        float y = uv.y * height - 0.5f;

        int i = (int)glm::floor(x);
        int j = (int)glm::floor(y);

        pii imgPos0{i, j};
        pii imgPos1{imgPos0.first +1, imgPos0.second};
        pii imgPos2{imgPos0.first, imgPos0.second+1};
        pii imgPos3{imgPos0.first + 1, imgPos0.second+1};
        float wx0 = x - imgPos0.first;
        float wx1 = 1.0f - wx0;
        float wy0 = y - imgPos0.second;
        float wy1 = 1.0f - wy0;

        glm::vec3 color0 = GetWrapped(imgPos0.first, imgPos0.second) * wx1 +
                           GetWrapped(imgPos1.first, imgPos1.second) * wx0;
        glm::vec3 color1 = GetWrapped(imgPos2.first, imgPos2.second) * wx1 +
                           GetWrapped(imgPos3.first, imgPos3.second) * wx0;

        return (color0 * wy1 + color1 * wy0);
    }
};