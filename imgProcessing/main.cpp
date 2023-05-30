
#include <omp.h>
#include <string>
#include <iostream>
#include "Raytracer.h"
#include "Texture.h"

int main(int argc, char** argv) {
	const int width = 1280, height = 720;
	
	std::vector<glm::vec3> pixels(width * height);
	std::string filename;
	std::cout << "저장 파일 이름: ";
	std::cin >> filename;

	son::Raytracer raytracer(width,height);
	raytracer.Render(pixels);
	
	Texture img(width,height,3, pixels);
	img.WritePng(filename);
	
}