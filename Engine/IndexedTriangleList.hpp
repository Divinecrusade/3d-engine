#pragma once

#include <vector>
#include "Vec3.h"

struct IndexedTriangleList
{
    std::vector<Vec3> vertices{ };
    std::vector<std::size_t> indices{ };
};