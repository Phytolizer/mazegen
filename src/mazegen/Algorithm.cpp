#include "Algorithm.hpp"

std::optional<Algorithm> ParseAlgorithm(const std::string_view s)
{
    if (s == "prims")
    {
        return Algorithm::prims;
    }
    if (s == "random_dfs")
    {
        return Algorithm::random_dfs;
    }

    return std::nullopt;
}

std::string AlgorithmName(const Algorithm a)
{
    switch (a)
    {
    case Algorithm::prims:
        return "Prim's";
    case Algorithm::random_dfs:
        return "random DFS";
    }
    // Unreachable.
    return {};
}
