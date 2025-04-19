#pragma once

#include "IndexedTriangleList.hpp"

class Plain
{
public:

    template<class Vertex>
    static IndexedTriangleList<Vertex> GetNonSkinned(std::size_t triangles_in_row, float size = 1.f)
    {
        std::vector<Vertex> model{ };
        std::vector<std::size_t> indices{ };
        std::size_t const triangles_total{ 1ull << (2ull * triangles_in_row) };
        std::size_t const vertices_total{ 3ull * triangles_total };
        model.reserve(vertices_total);
        indices.reserve(vertices_total);

        float const cathetus_length{ size / triangles_in_row };
        float y{ size / 2.f };
        constexpr float z{ 0.f };

        for (std::size_t i{ 0ull }; i != triangles_in_row; ++i, y -= cathetus_length)
        {
            float x{ -size / 2.f };
            for (std::size_t j{ 0ull }; j != triangles_in_row; ++j, x += cathetus_length)
            {
                indices.emplace_back(model.size());
                model.emplace_back(Vec3{ x, y, z }); // left-top | size-6
                indices.emplace_back(model.size());
                model.emplace_back(Vec3{ x + cathetus_length, y, z }); // right-top | size-5
                indices.emplace_back(model.size());
                model.emplace_back(Vec3{ x, y - cathetus_length, z }); // left-bottom | size-4

                indices.emplace_back(model.size());
                model.emplace_back(Vec3{ x + cathetus_length, y, z }); // right-top | size-3
                indices.emplace_back(model.size());
                model.emplace_back(Vec3{ x + cathetus_length, y - cathetus_length, z }); // right-bottom | size-2
                indices.emplace_back(model.size());
                model.emplace_back(Vec3{ x, y - cathetus_length, z }); // left-bottom | size-1

                indices.emplace_back(model.size() - 6ull); // left-top
                indices.emplace_back(model.size() - 4ull); // left-bottom
                indices.emplace_back(model.size() - 5ull); // right-top

                indices.emplace_back(model.size() - 3ull); // right-top
                indices.emplace_back(model.size() - 1ull); // left-bottom
                indices.emplace_back(model.size() - 2ull); // right-bottom
            }
        }

        return
            IndexedTriangleList<Vertex> {
                std::move(model),
                std::move(indices)
            };
    }


    template<class Vertex>
    static IndexedTriangleList<Vertex> GetSkinned(std::size_t triangles_in_row, float size = 1.f)
    {
        auto plain{ GetNonSkinned<Vertex>(triangles_in_row, size) };

        Vec2 texture_pos{ 0.f, 0.f };
        float const cathetus_length{ 1.f / triangles_in_row };

        auto it{ plain.vertices.begin() };
        for (std::size_t i{ 0ull }; i != triangles_in_row; ++i, texture_pos.y += cathetus_length)
        {
            texture_pos.x = 0.f;
            for (std::size_t j{ 0ull }; j != triangles_in_row; ++j, texture_pos.x += cathetus_length)
            {
                it->texture_pos = texture_pos;
                ++it;
                it->texture_pos = Vec2{ texture_pos.x + cathetus_length, texture_pos.y };
                ++it;
                it->texture_pos = Vec2{ texture_pos.x, texture_pos.y + cathetus_length };
                ++it;

                it->texture_pos = Vec2{ texture_pos.x + cathetus_length, texture_pos.y };
                ++it;
                it->texture_pos = Vec2{ texture_pos.x + cathetus_length, texture_pos.y + cathetus_length };
                ++it;
                it->texture_pos = Vec2{ texture_pos.x, texture_pos.y + cathetus_length };
                ++it;
            }
        }
    
        return std::move(plain);
    }
};