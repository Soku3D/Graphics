#pragma once
#include "Raytracer.h"

Raytracer::Raytracer(int width, int height)
    : width(width),
      height(height),
      aspect((float)width / height)
{
    auto sphere = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 1.5f), 0.8f);

    sphere->amb = glm::vec3(0.2f);
    sphere->dif = glm::vec3(0.8f, 0.3f, 0.3f);
    sphere->spec = glm::vec3(1.0f);
    sphere->alpha = 100.0f;
    sphere->ks = 1.0f;
    objects.push_back(sphere);

    auto triangle = std::make_shared<Triangle>(glm::vec3(-2.0f, -2.0f, 2.0f),
                                               glm::vec3(-2.0f, 2.0f, 2.0f),
                                               glm::vec3(2.0f, 2.0f, 2.0f));

    triangle->amb = glm::vec3(0.1f);
    triangle->dif = glm::vec3(0.8f);
    triangle->spec = glm::vec3(1.0f);
    triangle->alpha = 100.0f;
    triangle->ks = 1.0f;
    objects.push_back(triangle);

    light.pos = glm::vec3(0.0f, -1.4f, -1.0f);
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
glm::vec3 Raytracer::Raytrace(const Ray& ray)
{
    Hit hit = FindClosestCollision(ray);
    if (hit.d >= 0.0f)
    {
        glm::vec3 ambColor;
        if (hit.obj->texture)
        {
            ambColor = hit.obj->texture->GetClamped(hit.uv);
        }
        else
        {
            ambColor = hit.obj->amb;
        }
       
        glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);
        glm::vec3 dirToEye = glm::normalize(ray.dir - hit.point);
        glm::vec3 reflectDir =
            2.0f * glm::dot(hit.norm, dirToLight) * hit.norm - dirToLight;

        Ray RayToLight{hit.point + hit.norm * 1e-5f, dirToLight};
        if (IsShadow(RayToLight))
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
void Raytracer::Render(std::vector<glm::vec3>& pixels)
{

    std::fill(pixels.begin(), pixels.end(), glm::vec3(0.0f));

#pragma omp parallel for
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            glm::vec3 worldPos = TransformScreenToWorld(glm::vec2(x, y));
            Ray ray{worldPos, glm::normalize(worldPos - eyePos)};
            pixels[y * width + x] = glm::clamp(Raytrace(ray), 0.0f, 1.0f);
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