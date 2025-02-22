#pragma once

#include "IndexedLineList.h"


class Cube
{
public:

    Cube(float size)
    { 
        auto const half_size{ size / 2.f };

        model.emplace_back(-half_size, half_size, half_size);
        model.emplace_back(half_size, half_size, half_size);
        model.emplace_back(-half_size, half_size, -half_size);
        model.emplace_back(half_size, half_size, -half_size);

        model.emplace_back(-half_size, -half_size, half_size);
        model.emplace_back(half_size, -half_size, half_size);
        model.emplace_back(-half_size, -half_size, -half_size);
        model.emplace_back(half_size, -half_size, -half_size);
    }

    IndexedLineList GetLines() const
    {
        return IndexedLineList
        {
            model,
            { 0U,1U, 0U,2U, 0U,4U, 1U,3U, 1U,5U, 2U,3U, 2U,6U, 4U,5U, 3U,7U, 7U,6U, 7U,5U }
        };
    }

private:

    std::vector<Vec3> model{ };
};