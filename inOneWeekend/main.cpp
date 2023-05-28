#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include "Raytracer.h"


std::ofstream& operator<<(std::ofstream& in, glm::vec3 vec) {
	in << static_cast<int>(vec.x * 255.999)
		<< ' ' << static_cast<int>(vec.y * 255.999)
		<< ' ' << static_cast<int>(vec.z * 255.999) << '\n';
	return in;
}
int main() {

	const int image_width = 1280;
	const int image_height = 720;
	son::Raytracer raytracer(image_width, image_height);

	std::ofstream image("C:/Users/s9912/Desktop/images/OneSphere.ppm");
	std::vector<glm::vec3> pixels;
	pixels.resize(image_width * image_height);
	image << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	raytracer.Render(pixels);
	if (image.is_open()) {
		for (int y = 0; y < image_height; y++)
		{
			for (int x = 0; x < image_width; x++)
			{
				image << pixels[y * image_width + x];
			}
		}
	}
	else {
		std::cout << "error";
	}
	image.close();

	return 0;
} 