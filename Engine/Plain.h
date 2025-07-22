#pragma once

#include "IndexedTriangleList.hpp"

class Plain {
 public:
  template <class Vertex>
  static IndexedTriangleList<Vertex> GetNonSkinned(std::size_t divisions_x,
                                                   std::size_t divisions_y,
                                                   float width = 1.f,
                                                   float height = 1.f) {
    auto const vertices_x_count{divisions_x + 1};
    auto const vertices_y_count{divisions_y + 1};
    std::vector<Vertex> vertices(vertices_x_count * vertices_y_count);


    float const side_x{width / 2.f};
    float const side_y{height / 2.f};
    float const division_size_x{width / float(divisions_x)};
    float const division_size_y{height / float(divisions_y)};
    Vec3 const bottom_left{-side_x, -side_y, 0.f};

    for (auto y{0ull}, i{0ull}; y != vertices_y_count; ++y) {
      float const y_pos{float(y) * division_size_y};
      for (auto x{0ull}; x != vertices_x_count; ++x, ++i) {
        vertices[i].model_pos =
            bottom_left + Vec3{float(x) * division_size_x, y_pos, 0.f};
      }
    }

    std::vector<std::size_t> indices{};
    indices.reserve(divisions_x * divisions_y * divisions_x * divisions_y *
                    6ull);
    auto const vxy2i = [vertices_x_count](std::size_t x, std::size_t y) {
      return y * vertices_x_count + x;
    };
    for (std::size_t y{0ull}; y != divisions_y; ++y) {
      for (std::size_t x{0ull}; x != divisions_x; ++x) {
        auto const left_bottom{vxy2i(x, y)};
        auto const right_bottom{vxy2i(x + 1ull, y)};
        auto const left_top{vxy2i(x, y + 1ull)};
        auto const right_top{vxy2i(x + 1ull, y + 1ull)};

        indices.push_back(left_bottom);
        indices.push_back(left_top);
        indices.push_back(right_bottom);

        indices.push_back(right_bottom);
        indices.push_back(left_top);
        indices.push_back(right_top);
      }
    }

    return IndexedTriangleList<Vertex>{std::move(vertices), std::move(indices)};
  }

  template <class Vertex>
  static IndexedTriangleList<Vertex> GetNonSkinnedWithNormals(
      std::size_t divisions_x, 
      std::size_t divisions_y, 
      float width,
      float height) {
    auto plain{GetNonSkinned<Vertex>(divisions_x, divisions_y, width, height)};
    for (auto& vertex : plain.vertices) {
      vertex.n = Vec3{0.f, 0.f, 1.f};
    }
    return plain;
  }

  template <class Vertex>
  static IndexedTriangleList<Vertex> GetSkinnedWithNormals(std::size_t divisions_x,
                                                std::size_t divisions_y,
                                                float width = 1.f,
                                                float height = 1.f,
                                                float texture_scale = 1.f) {
    auto plain{GetNonSkinnedWithNormals<Vertex>(divisions_x, 
                                                divisions_y, 
                                                width,
                                                height)};

    
    auto const vertices_x_count{divisions_x + 1};
    auto const vertices_y_count{divisions_y + 1};

    float const division_size_x{width / float(divisions_x)};
    float const division_size_y{height / float(divisions_y)};

    Vec2 const bottom_left_texture{0.f, 1.f};

    for (auto y{0ull}, i{0ull}; y != vertices_y_count; ++y) {
      float const y_texture_pos{-float(y) * division_size_y / texture_scale};
      for (auto x{0ull}; x != vertices_x_count; ++x, ++i) {
        plain.vertices[i].texture_pos =
            bottom_left_texture +
            Vec2{float(x) * division_size_x, y_texture_pos};
      }
    }

    return plain;
  }
};