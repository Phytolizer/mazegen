#pragma once
#include <tuple>
#include <vector>

namespace gen
{
struct Edge
{
    float x1;
    float y1;
    float x2;
    float y2;
};
} // namespace gen

std::pair<std::vector<float>, std::vector<float>> GeneratePoints(std::size_t n);
