#ifndef CAVE_H
#define CAVE_H

// #include <concurrent_vector.h>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stack>
#include <time.h>

// #include <omp.h>
#include "./bidirectional_traversal_generation/plot.hpp"

// using namespace concurrency;

typedef struct {
    float x, y;
} vector2;

class Cave {
private:
    int64_t _width, _height;
    float** _matrix;

public:
    Cave() = delete;
    Cave(const int64_t& w, const int64_t& h);

    // bidirectional traversal demo
    Cave(const int64_t& w, const int64_t& h, bool** bwPixels)
    {
        _width = w;
        _height = h;

        _matrix = new float*[_height];

        for (int y = 0; y < _height; ++y) {
            _matrix[y] = new float[_width];
            for (int x = 0; x < _width; ++x)
                _matrix[y][x] = (float)bwPixels[y][x];
        }
    }

    Cave(BooleanMatrix bMatrix)
        : Cave(bMatrix.width, bMatrix.height, bMatrix.getPixels())
    {
    }

    // properties

    float** matrix() { return const_cast<float**>(_matrix); }
    float** matrix() const { return const_cast<float**>(_matrix); }

    inline auto width() { return _width; }
    inline auto height() { return _height; }

    inline auto height() const { return _height; }
    inline auto width() const { return _width; }

    // floatmap manipulation

    void noise(float lower, float upper);
    void box_blur(int radius);

    // DFS-based
    void carve(float criteria);

    // operators
    Cave& operator=(const Cave& rhs);

    float at(const int64_t& x, const int64_t& y);
    float at(const int64_t& x, const int64_t& y) const;

private:
    float interpolate(float a0, float a1, float w);
    vector2 random_gradient(int ix, int iy);
    float dotgrid_gradient(int ix, int iy, float x, float y);
    float perlin(float x, float y);
};

// Pixel, PixelMatrix, Bitmap
#include "./bitmap.h"

class CaveImage {
private:
    Cave _cave;
    PixelMatrix _pixelmap;

public:
    CaveImage() = delete;
    CaveImage(const Cave& cave);

    void load(const Cave& cave);
    void save(const std::string& filename);
    void scale(const uint64_t& factor);

private:
    void init_pixelmap();
};

#endif