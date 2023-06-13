#pragma once
#include "Raytracer.h"

Raytracer::Raytracer(int width, int height)
    : width(width),
      height(height),
      aspect((float)width / height)
{
    auto texture0 = std::make_shared<Texture>("texture0");
    // auto texturetest = std::make_shared<Texture>("texturetest");

    auto sphere = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 1.5f), 0.8f);

    sphere->amb = glm::vec3(0.2f);
    sphere->dif = glm::vec3(0.8f, 0.3f, 0.3f);
    sphere->spec = glm::vec3(1.0f);
    sphere->alpha = 100.0f;
    sphere->ks = 1.0f;

    auto triangle = std::make_shared<Triangle>(glm::vec3(-2.0f, -2.0f, 2.0f),
                                               glm::vec3(-2.0f, 2.0f, 2.0f),
                                               glm::vec3(2.0f, 2.0f, 2.0f));

    triangle->amb = glm::vec3(0.1f);
    triangle->dif = glm::vec3(0.8f);
    triangle->spec = glm::vec3(1.0f);
    triangle->alpha = 100.0f;
    triangle->ks = 1.0f;

    auto rectangle = std::make_shared<Rectangle>(
        glm::vec3(-2.0f, -2.0f, 1.0f), glm::vec3(-2.0f, -2.0f, 4.0f),
        glm::vec3(2.0f, -2.0f, 4.0f), glm::vec3(2.0f, -2.0f, 1.0f), 4.0f);

    rectangle->amb = glm::vec3(0.1f);
    rectangle->dif = glm::vec3(0.8f);
    rectangle->spec = glm::vec3(1.0f);
    rectangle->alpha = 100.0f;
    rectangle->ks = 1.0f;
    rectangle->texture = texture0;

    auto rectangle2 = std::make_shared<Rectangle>(
        glm::vec3(-2.0f, -2.0f, 2.0f), glm::vec3(-2.0f, 2.0f, 2.0f),
        glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, -2.0f, 2.0f), 4.0f);

    rectangle2->amb = glm::vec3(0.1f);
    rectangle2->dif = glm::vec3(0.8f);
    rectangle2->spec = glm::vec3(1.0f);
    rectangle2->alpha = 100.0f;
    rectangle2->ks = 1.0f;
    rectangle2->texture = texture0;

    objects.push_back(triangle);
    // objects.push_back(rectangle);
    // objects.push_back(rectangle2);
    objects.push_back(sphere);

    light.pos = glm::vec3(0.0f, 2.4f, 0.0f);
    eyePos = glm::vec3(0.0f, 0.0f, -1.0f);
}
Hit Raytracer::FindClosestCollision(const Ray& ray)
{
    Hit closestHit{-1.0f};
    float closestD = 1e10;
    for (int i = 0; i < objects.size(); i++)
    {
        Hit hit = objects[i]->IntersectRayCollision(ray);
        if (hit.d >= 0.0f && hit.d < closestD)
        {
            closestD = hit.d;
            closestHit = hit;
            closestHit.obj = objects[i];
        }
    }
    return closestHit;
}
bool Raytracer::IsShadow(const Ray& ray)
{
    Hit hit = FindClosestCollision(ray);
    if (hit.d >= 0.0f)
    {
        return true;
    }
    return false;
}
glm::vec3 Raytracer::traceRay(const Ray& ray)
{
    Hit hit = FindClosestCollision(ray);
    if (hit.d >= 0.0f)
    {
        glm::vec3 ambColor;
        if (hit.obj->texture)
        {
            ambColor = hit.obj->texture->GetLinear(hit.uv);
            return ambColor;
        }
        else
        {
            ambColor = hit.obj->amb;
        }

        glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);
        glm::vec3 dirToEye = glm::normalize(ray.dir - hit.point);
        glm::vec3 reflectDir =
            2.0f * glm::dot(hit.norm, dirToLight) * hit.norm - dirToLight;

        Ray shadowRay{hit.point + hit.norm * 1e-5f, dirToLight};
        if (IsShadow(shadowRay))
        {
            return ambColor;
        }

        float dif = glm::max(glm::dot(hit.norm, dirToLight), 0.0f);
        float spec = glm::pow(glm::max(glm::dot(reflectDir, -ray.dir), 0.0f),
                              hit.obj->alpha);

        return ambColor + dif * hit.obj->dif +
               spec * hit.obj->spec * hit.obj->ks;
    }
    return glm::vec3(0.0f);
}
glm::vec3 Raytracer::traceRay2x2(glm::vec3& currPos, float currD, int count)
{
    if (count == 0)
    {
        Ray ray{currPos, glm::normalize(currPos - eyePos)};
        return traceRay(ray); 
    }
    float subD = currD * 0.5;
    glm::vec3 p0 = currPos - glm::vec3(0.5f * subD, 0.5f * subD, 0.0f);
    //glm::vec3 color = traceRay2x2(currPos, currD,0);
    glm::vec3 color(0.0f);
    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x < 2; x++)
        {
            glm::vec3 p = p0 + glm::vec3(subD * x, subD * y, 0.0f);
            color += traceRay2x2(p, subD, count - 1);
        }
    }
    color *= 0.25;
    return glm::clamp(color, 0.0f, 1.0f);
}
void Raytracer::Render(std::vector<glm::vec3>& pixels)
{

    std::fill(pixels.begin(), pixels.end(), glm::vec3(0.0f));

#pragma omp parallel for
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            glm::vec3 worldPos = TransformScreenToWorld(glm::vec2(x, y));
            float d = 2.0f / height;
            pixels[y * width + x] = glm::clamp(traceRay2x2(worldPos, d, 3), 0.0f, 1.0f);
            // std::cout << x << ' ' << y << '\n';
        }
    }
}
glm::vec3 Raytracer::TransformScreenToWorld(const glm::vec2& pos)
{
    float xScale = 2.0f / width;
    float yScale = 2.0f / height;
    float x = pos.x * xScale;
    float y = pos.y * yScale;
    return glm::vec3{(x - 1.0f) * aspect, -(y - 1.0f), 0.0f};
}