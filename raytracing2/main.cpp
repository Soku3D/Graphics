#include "Texture.h"
#include "Raytracer.h"
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <filesystem>
namespace fs = std::filesystem;

#define STRINGIZING(x) #x

#define EXPAND(x) STRINGIZING(x)
std::string resultPath;
std::string resultFilename;
std::string directoryName;

void initialize()
{
    resultPath = EXPAND(RESULTS);
    resultPath.erase(0, 1);                  // erase the first quote
    resultPath.erase(resultPath.size() - 2); // erase the last quote and the dot
    std::cout << "Enter Saving Directory: ";
    std::cin >> directoryName;
    std::cout << "Enter Rendering Img filename : ";
    std::cin >> resultFilename;
    resultPath += directoryName + "\\" + resultFilename;
    std::string path = "Results/" + directoryName;
    fs::create_directories(path);
}
int main(int argc, char** argv)
{

    initialize();
    int width = 1920, height = 1080;
    Raytracer raytracer(width, height);
    std::vector<glm::vec3> pixels(width * height);
    raytracer.Render(pixels);
    Texture texture(width, height, 3, pixels);
    texture.RenderImage(resultFilename, directoryName);
    std::string filePath = resultPath + ".png";
    system(filePath.c_str());

}