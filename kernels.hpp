#pragma once

#include <iostream>
#include <vector>

// clang-format off

namespace Kernels {
// Box-blur kernels
std::vector<float> box_blur_3x3(9, 0.111f);
std::vector<float> box_blur_6x6(36, 0.111f);
std::vector<float> box_blur_9x9(81, 0.111f);

// Invert kernels
std::vector<int> invert_3x3 = {0, 0, 0, 0, -1, 0, 0, 0, 0};
std::vector<int> invert_5x5 = {0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0};
std::vector<int> invert_7x7 = {0,0,0,0,0,0,0, 0,0,0,-1,0,0,0, 0,0,0,0,0,0,0,};

} // namespace Kernels

// clang-format on