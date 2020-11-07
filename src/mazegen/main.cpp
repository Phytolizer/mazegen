#include "Algorithm.hpp"
#include "Delaunay.hpp"
#include "Generation.hpp"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <VoronoiDiagramGenerator.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/program_options.hpp>
#include <boost/range/combine.hpp>
#include <delaunay.h>
#include <exception>
#include <iostream>
#include <random>
#include <unordered_set>

using boost::numeric::ublas::triangular_matrix;
using boost::numeric::ublas::upper;

namespace po = boost::program_options;

int main(const int argc, char *argv[])
{
    std::random_device rd;
    std::default_random_engine eng(rd());

    auto desc = po::options_description{"Options"};

    desc.add_options()("help", "print this help message")(
        "algorithm", po::value<std::string>(),
        "select algorithm (default=random DFS)")(
        "num-points", po::value<int>(),
        "select number of points (default=random)")(
        "num-exits", po::value<int>(), "select number of exits (default=1)");

    auto vm = po::variables_map{};
    try
    {
        store(parse_command_line(argc, argv, desc), vm);
    }
    catch (std::exception &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    if (vm.count("help") > 0)
    {
        std::cout << desc << "\n";
        return 0;
    }

    Algorithm algorithm;
    if (vm.count("algorithm") > 0)
    {
        auto alg = ParseAlgorithm(vm["algorithm"].as<std::string>());
        if (alg.has_value())
        {
            algorithm = alg.value();
            std::cout << "Selecting " << AlgorithmName(algorithm)
                      << " algorithm.\n";
        }
        else
        {
            std::cerr << "Unsupported algorithm "
                      << vm["algorithm"].as<std::string>() << ".\n";
            return 1;
        }
    }
    else
    {
        std::cout << "No algorithm selected, using random DFS.\n";
        algorithm = Algorithm::random_dfs;
    }

    int n = 0;
    if (vm.count("num-points") > 0)
    {
        n = vm["num-points"].as<int>();
        if (n <= 0)
        {
            std::cerr << "Cannot generate a maze using " << n << " points.\n";
            return 1;
        }
    }
    else
    {
        std::cout << "Number of points not specified, using random number.\n";
        n = std::uniform_int_distribution<int>(10, 20)(eng);
    }
    std::cout << "Generating maze using " << n << " points.\n";

    auto [xs, ys] = GeneratePoints(n);
    VoronoiDiagramGenerator generator;
    generator.generateVoronoi(xs.data(), ys.data(), static_cast<int>(xs.size()),
                              0, 1, 0, 1, 0.01F);
    generator.resetIterator();
    gen::Edge edge{};
    std::vector<gen::Edge> edges;
    while (generator.getNext(edge.x1, edge.y1, edge.x2, edge.y2))
    {
        edges.push_back(edge);
    }

    std::unordered_set<del_point2d_t> points;
    constexpr auto windowWidth = 800;
    constexpr auto windowHeight = 600;
    for (auto [x, y] : boost::combine(xs, ys))
    {
        points.insert({static_cast<double>(x * windowWidth),
                       static_cast<double>(y * windowHeight)});
    }
    std::vector<del_point2d_t> pointsVec;
    std::copy(points.begin(), points.end(), std::back_inserter(pointsVec));

    Delaunay delaunay{pointsVec};
    SDL_Init(SDL_INIT_VIDEO);

    auto *window = SDL_CreateWindow(
        "Output", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
        windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    auto *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    auto run = true;
    while (run)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                run = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        for (auto &edge : edges)
        {
            SDL_RenderDrawLine(renderer, edge.x1 * windowWidth,
                               edge.y1 * windowHeight, edge.x2 * windowWidth,
                               edge.y2 * windowHeight);
        }
        delaunay.Draw(renderer);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
