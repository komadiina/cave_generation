#include "./cave.h"
#include <iostream>

int main(int argC, char** argV) {
	int64_t width = 16, height = 16;
	float criteria = 0.3666;

	if (argC == 3) {
		width = height = std::stoi(argV[1]);
		criteria = std::stof(argV[2]);
	} else if (argC == 4) {
		width = std::stoi(argV[1]);
		height = std::stoi(argV[2]);
		criteria = std::stof(argV[3]);
	} else if (argC != 1) {
		std::cout << "Invalid commandline arguments (expected 1, 3 or 4, got " << argC << ")." << std::endl;
		exit(1);
	}

	printf("Resolution: %dx%d\nCriteria: %f\n", width, height, criteria);

	Cave cavesystem(width, height);
	cavesystem.noise(0, 1);
	cavesystem.carve(criteria);
	cavesystem.box_blur(1);

	CaveImage output(cavesystem);
	output.save("examples/original_cave.bmp");

	return EXIT_SUCCESS;
}