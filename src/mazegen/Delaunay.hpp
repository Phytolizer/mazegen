#pragma once

#include "IDrawable.hpp"
#include "SDL2/SDL_render.h"
#include <boost/numeric/ublas/triangular.hpp>
#include <delaunay.h>
#include <vector>

struct Face
{
    std::vector<del_point2d_t> vertices;
};

class Delaunay : public IDrawable
{
    std::vector<del_point2d_t> m_points;
    boost::numeric::ublas::triangular_matrix<bool> m_connections;

  public:
    explicit Delaunay(std::vector<del_point2d_t> &points) noexcept;

    [[nodiscard]] const std::vector<del_point2d_t> &Points() const noexcept;
    [[nodiscard]] const boost::numeric::ublas::triangular_matrix<bool>
        &Connections() const noexcept;

    void Draw(SDL_Renderer *renderer) noexcept override;
};

bool operator==(const del_point2d_t &p1, const del_point2d_t &p2);

namespace std
{
template <> struct hash<del_point2d_t>
{
    std::size_t operator()(const del_point2d_t &p) const noexcept
    {
        auto h1 = std::hash<double>{}(p.x);
        auto h2 = std::hash<double>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};
} // namespace std
