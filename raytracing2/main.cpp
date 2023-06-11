#include "Texture.h"
#include <algorithm>
#include <windows.h>
#define STRINGIZING(x) #x

#define EXPAND(x) STRINGIZING(x)
std::string resultPath;
std::string resultFilename;

void initialize()
{
    resultPath = EXPAND(RESULTS);
    resultPath.erase(0, 1);                  // erase the first quote
    resultPath.erase(resultPath.size() - 2); // erase the last quote and the dot
    std::cout << "Enter Rendering Img filename : ";
    std::cin >> resultFilename;
}
int main(int argc, char** argv)
{
    initialize();
    int width = 1920, height = 1080;
    Texture texture;
    texture.RenderImage(resultFilename);
    std::string filePath = resultPath + resultFilename + ".png";
    system(filePath.c_str());
}