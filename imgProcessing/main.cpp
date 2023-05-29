
#include <omp.h>
#include <string>
#include <iostream>
#include "Raytracer.h"
#include "Texture.h"

int main(int argc, char** argv) {
	int width = 1920, height = 1080;
	
	//std::vector<glm::vec3> pixels(width * height);
	std::string filename;
	std::cout << "저장 파일 이름: ";
	std::cin >> filename;

	son::Raytracer raytracer(width,height);
	//raytracer.Render(pixels);
	std::vector<glm::vec3> pixels;
	Texture img(0,0,0,filename);
	img.ReadFromImage(pixels);
	img.WritePng(pixels);
	
}