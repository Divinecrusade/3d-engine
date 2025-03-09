#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include "Vec3.h"

struct IndexedTriangleList
{
    IndexedTriangleList(std::vector<Vec3> init_vertices, std::vector<std::size_t> init_indices)
    :
    vertices{ std::move(init_vertices) },
    indices{ std::move(init_indices) },
    cullFlags(indices.size() / 3U)
    {
        assert(vertices.size() >= 3U);
        assert(indices.size() % 3U == 0U);
    }

    std::vector<Vec3> vertices{ };
    std::vector<std::size_t> indices{ };
    std::vector<bool> cullFlags{ };
};