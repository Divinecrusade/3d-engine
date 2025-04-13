#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include "Vec3.h"

template<typename T = Vec3>
struct IndexedTriangleList
{
    IndexedTriangleList(std::vector<T> init_vertices, std::vector<std::size_t> init_indices)
    :
    vertices{ std::move(init_vertices) },
    indices{ std::move(init_indices) }
    {
        assert(vertices.size() >= 3U);
        assert(indices.size() % 3U == 0U);
    }

    std::vector<T> vertices{ };
    std::vector<std::size_t> indices{ };
};