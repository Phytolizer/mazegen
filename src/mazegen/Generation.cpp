#include "Generation.hpp"
#include <random>

std::pair<std::vector<float>, std::vector<float>> GeneratePoints(const std::size_t n)
{
    std::random_device rd;
    const std::uniform_real_distribution<float> dist{0, 1};

    std::vector<float> xs;
    std::vector<float> ys;
    for (std::size_t i = 0; i < n; i++)
    {
        xs.push_back(dist(rd));
        ys.push_back(dist(rd));
    }
    return {xs, ys};
}

