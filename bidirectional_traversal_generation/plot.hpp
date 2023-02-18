#ifndef _IMAGE_HXX
#define _IMAGE_HXX

#include "../bitmap.h"
#include <iostream>

class BooleanMatrix {
private:
    bool** pixels;

public:
    const int width, height;

public:
    BooleanMatrix(int w, int h)
        : width(w)
        , height(h)
    {
        pixels = new bool*[h];
        for (int i = 0; i < h; i++)
            pixels[i] = new bool[w];

        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                pixels[y][x] = false;
    }

    bool** getPixels() { return pixels; }
    bool** getPixels() const { return pixels; }

    BooleanMatrix(int dim)
        : BooleanMatrix(dim, dim)
    {
    }

    void randomize(int iterations = 0, int x_offset = 0, int y_offset = 0)
    {
        if (iterations == 0)
            iterations = width * width + height * height / 2;

        srand(time(NULL));
        clear_matrix();

        int posX = width / 2 + x_offset, posY = height / 2 + y_offset;
        while (iterations--) {
            // (randX, randY)
            int directionX = rand() % 3 - 1;
            int directionY = rand() % 3 - 1;

            while (validX(posX + directionX) == false)
                directionX = rand() % 2;

            while (validY(posY + directionY) == false)
                directionY = rand() % 2;

            posX += directionX;
            posY += directionY;

            pixels[posY][posX] = true;
        }
    }

    PixelMatrix to_pixels()
    {
        std::vector<std::vector<Pixel>> pixelmap;

        for (int y = 0; y < height; ++y) {
            std::vector<Pixel> row;

            for (int x = 0; x < width; ++x)
                row.push_back(Pixel(0));

            pixelmap.push_back(row);
        }

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                pixelmap[y][x] = Pixel((int)pixels[y][x] * 255);

        return pixelmap;
    }

private:
    void clear_matrix()
    {
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                pixels[y][x] = false;
    }

    bool validX(int x)
    {
        return x && x < width;
    }

    bool validY(int y)
    {
        return y && y < height;
    }
};

#endif