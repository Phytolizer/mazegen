#include "Delaunay.hpp"
#include <delaunay.h>

Delaunay::Delaunay(std::vector<del_point2d_t> &points) noexcept
{
    auto *data = delaunay2d_from(points.data(), points.size());
    m_points = std::vector<del_point2d_t>{data->points,
                                          data->points + data->num_points};
    int i = 0;
    m_connections = boost::numeric::ublas::triangular_matrix<bool>(
        data->num_faces, data->num_faces);
    m_connections.clear();
    for (int face = 0; face < data->num_faces; face++)
    {
        for (int v1 = 1; v1 <= data->faces[i]; v1++)
        {
            for (int v2 = 1; v2 < v1; v2++)
            {
                auto r = data->faces[v1];
                auto c = data->faces[v2];
                if (c > r)
                {
                    auto temp = r;
                    r = c;
                    c = temp;
                }
                m_connections.at_element(r, c) = true;
            }
        }
        i += data->faces[i] + 1;
    }
    // m_faces = std::vector<unsigned int>{data->faces, data->faces + i};
    delaunay2d_release(data);
}

const std::vector<del_point2d_t> &Delaunay::Points() const noexcept
{
    return m_points;
}

void Delaunay::Draw(SDL_Renderer *renderer) noexcept
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    for (int i = 0; i < m_points.size(); i++)
    {
        for (int c = 0; c < i; c++)
        {
            if (m_connections.at_element(i, c))
            {
                SDL_RenderDrawLine(renderer, m_points[i].x, m_points[i].y,
                                   m_points[c].x, m_points[c].y);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    for (const auto &point : m_points)
    {
        SDL_Rect rect = {static_cast<int>(point.x) - 5,
                         static_cast<int>(point.y) - 5, 10, 10};
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool operator==(const del_point2d_t &p1, const del_point2d_t &p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}
