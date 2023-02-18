#include "../bitmap.h"
#include "./plot.hpp"

#include <Windows.h>
#include <cmath>

int main(int argC, char** argV)
{
    int w = std::stoi(argV[1]);
    int h = std::stoi(argV[2]);
    int n = std::stoi(argV[3]);

    std::string dim = std::string(argV[1]) + "x" + std::string(argV[2]);

    while (n--) {
        BooleanMatrix cave(w, h);
        cave.randomize(0, w / 4, h / 4);

        Bitmap bmp;
        bmp.fromPixelMatrix(cave.to_pixels());
        bmp.save(dim + "_cave" + std::to_string(n + 1) + ".bmp");

        Sleep(1001);
    }

    return EXIT_SUCCESS;
}