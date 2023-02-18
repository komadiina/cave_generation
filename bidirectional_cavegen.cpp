#include "./bidirectional_traversal_generation/plot.hpp"
#include "./cave.h"

// Sleep() for srand
#include <Windows.h>

int main(int argC, char** argV)
{
    int w = std::stoi(argV[1]);
    int h = std::stoi(argV[2]);
    int n = std::stoi(argV[3]);
    int r = std::stoi(argV[4]);

    std::string dim = std::string(argV[1]) + "x" + std::string(argV[2]);

    while (n--) {
        std::string output_name = dim + "_cave" + std::to_string(n + 1) + ".bmp";

        BooleanMatrix booleanCave(w, h);
        booleanCave.randomize(0, w / 4, h / 4);

        Bitmap bmp;
        bmp.fromPixelMatrix(booleanCave.to_pixels());
        bmp.save(output_name);

        Sleep(1001);

        Cave cave(booleanCave);
        cave.box_blur(r);

        CaveImage cave_image(cave);
        cave_image.save("blurred_" + output_name);
    }

    return EXIT_SUCCESS;
}