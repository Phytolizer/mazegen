#pragma once

#include <optional>
#include <string_view>

enum class Algorithm
{
    prims,
    random_dfs,
};

std::optional<Algorithm> ParseAlgorithm(std::string_view s);

std::string AlgorithmName(Algorithm a);

