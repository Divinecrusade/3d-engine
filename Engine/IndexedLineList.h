#pragma once

#include <vector>
#include "Vec3.h"
#include <vector>

struct IndexedLineList
{
    std::vector<Vec3> vertices{ };
    std::vector<std::size_t> indices{ };
};