#include "Texture.h"
#include <windows.h>
#include <algorithm>
#define STRINGIFY(x) #x

#define EXPAND(x) STRINGIFY(x)

std::string resultPath;
void SetResultPath() {
	resultPath = EXPAND(RESULTS);
	resultPath.erase(0, 1); // erase the first quote
	resultPath.erase(resultPath.size() - 2); // erase the last quote and the dot
}
int main(int argc, char** argv) {
	SetResultPath();
	std::string resultFilename;
	std::cout << "Enter Rendering Img filename : ";
	std::cin >> resultFilename;
	Texture texture;
	texture.RenderImage(resultFilename);
	std::string filePath = resultPath + resultFilename + ".png";
	system(filePath.c_str());
}