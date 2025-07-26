#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <memory>

#include "ChiliMath.h"
#include "Graphics.h"
#include "IndexedTriangleList.hpp"

template <class GraphicEffect, class Vertex = GraphicEffect::Vertex,
          class VSV = GraphicEffect::VertexShader::OutVertex,
          class GSV = GraphicEffect::GeometryShader::OutVertex>
class PipelineDebug {
 public:
  using Triangle = std::array<GSV, 3u>;

 public:
  PipelineDebug(Graphics& gfx, GraphicEffect effect)
      : gfx{gfx},
        screen_half_width{gfx.ScreenWidth / 2.f},
        screen_half_height{gfx.ScreenHeight / 2.f},
        zbuffer{std::make_shared<std::unique_ptr<float[]>>(
            std::make_unique<float[]>(gfx.ScreenWidth * gfx.ScreenHeight))},
        effect{std::move(effect)} {}

  PipelineDebug(Graphics& gfx, GraphicEffect effect,
           std::shared_ptr<std::unique_ptr<float[]>> zbuffer)
      : gfx{gfx},
        screen_half_width{gfx.ScreenWidth / 2.f},
        screen_half_height{gfx.ScreenHeight / 2.f},
        zbuffer{zbuffer},
        effect{std::move(effect)} {}

  void BeginFrame() { ResetZBuffer(); }

  void Draw(IndexedTriangleList<Vertex> const& model) {
    ProcessVertices(model.vertices, model.indices);
  }

 private:
  void ProcessVertices(std::vector<Vertex> const& vertices,
                       std::vector<std::size_t> const& indices) {
    std::vector<VSV> processed{};

    std::transform(vertices.begin(), vertices.end(),
                   std::back_inserter(processed), effect.vs);

    for (auto& v : processed) {
      auto v0{v};
      auto v1{v};
      v1.model_pos = v1.nend;
      if (v0.model_pos.z < 0.01f || v1.model_pos.z < 0.01f) {
        continue;
      }
      TransformToNDC(v0);
      TransformToNDC(v1);
      gfx.DrawLineAndUpdateZBuffer(Vec3{v0.model_pos}, Vec3{v1.model_pos}, Colors::Cyan, 
          [this](int x, int y, float z) -> bool {
            return UpdateZBuffer({unsigned(x), unsigned(y)}, z);
          });
    }

    AssembleTriangles(processed, indices);
  }

  void AssembleTriangles(std::vector<VSV>& vertices,
                         std::vector<std::size_t> const& indices) {
    auto const eye{Vec4{0.f, 0.f, 0.f, 1.f} * effect.vs.GetProjection()};
    for (std::size_t i{0u}; i < indices.size(); i += 3u) {
      if ((vertices[indices[i + 1u]].model_pos -
           vertices[indices[i]].model_pos) %
              (vertices[indices[i + 2u]].model_pos -
               vertices[indices[i]].model_pos) *
              (vertices[indices[i]].model_pos - eye) >
          0.f)
        continue;

      ProccessTriangle(
          Triangle{effect.gs(vertices[indices[i]], vertices[indices[i + 1u]],
                             vertices[indices[i + 2u]], i / 3ull)});
    }
  }

  void ProccessTriangle(Triangle&& object) {
    ClipCullTriangle(std::forward<Triangle>(object));
  }

  void ClipCullTriangle(Triangle&& object) {
    if (object[0].model_pos.x > object[0].model_pos.w &&
        object[1].model_pos.x > object[1].model_pos.w &&
        object[2].model_pos.x > object[2].model_pos.w)
      return;
    if (object[0].model_pos.x < -object[0].model_pos.w &&
        object[1].model_pos.x < -object[1].model_pos.w &&
        object[2].model_pos.x < -object[2].model_pos.w)
      return;
    if (object[0].model_pos.y > object[0].model_pos.w &&
        object[1].model_pos.y > object[1].model_pos.w &&
        object[2].model_pos.y > object[2].model_pos.w)
      return;
    if (object[0].model_pos.y < -object[0].model_pos.w &&
        object[1].model_pos.y < -object[1].model_pos.w &&
        object[2].model_pos.y < -object[2].model_pos.w)
      return;
    if (object[0].model_pos.z > object[0].model_pos.w &&
        object[1].model_pos.z > object[1].model_pos.w &&
        object[2].model_pos.z > object[2].model_pos.w)
      return;
    if (object[0].model_pos.z < 0.f && object[1].model_pos.z < 0.f &&
        object[2].model_pos.z < 0.f)
      return;

    auto const Clip1{[this](GSV& v0, GSV& v1, GSV& v2) {
      auto const alphaA{(-v0.model_pos.z) / (v1.model_pos.z - v0.model_pos.z)};
      auto const alphaB{(-v0.model_pos.z) / (v2.model_pos.z - v0.model_pos.z)};
      auto v0A{interpolate(v0, v1, alphaA)};
      auto v0B{interpolate(v0, v2, alphaB)};
      PostProccessTriangle(Triangle{v0A, v1, v2});
      PostProccessTriangle(Triangle{v0B, v0A, v2});
    }};
    auto const Clip2{[this](GSV& v0, GSV& v1, GSV& v2) {
      auto const alpha0{(-v0.model_pos.z) / (v2.model_pos.z - v0.model_pos.z)};
      auto const alpha1{(-v1.model_pos.z) / (v2.model_pos.z - v1.model_pos.z)};
      v0 = interpolate(v0, v2, alpha0);
      v1 = interpolate(v1, v2, alpha1);
      PostProccessTriangle(Triangle{v0, v1, v2});
    }};

    if (object[0].model_pos.z < 0.f) {
      if (object[1].model_pos.z < 0.f) {
        Clip2(object[0], object[1], object[2]);
      } else if (object[2].model_pos.z < 0.f) {
        Clip2(object[0], object[2], object[1]);
      } else {
        Clip1(object[0], object[1], object[2]);
      }
    } else if (object[1].model_pos.z < 0.f) {
      if (object[2].model_pos.z < 0.f) {
        Clip2(object[1], object[2], object[0]);
      } else {
        Clip1(object[1], object[0], object[2]);
      }
    } else if (object[2].model_pos.z < 0.f) {
      Clip1(object[2], object[0], object[1]);
    } else {
      PostProccessTriangle(std::forward<Triangle>(object));
    }
  }

  void PostProccessTriangle(Triangle object) {
    for (auto& vertex : object) {
      TransformToNDC(vertex);
    }
    DrawTriangle(object);
  }

  template<class V>
  void TransformToNDC(V& v) {
    float const wInv{1.f / v.model_pos.w};
    v *= wInv;
    v.model_pos.x = v.model_pos.x * screen_half_width + screen_half_width;
    v.model_pos.y = -v.model_pos.y * screen_half_height + screen_half_height;
    v.model_pos.w = wInv;
  }

  void DrawTriangle(Triangle& object) {
    auto& p0{object[0u]};
    auto& p1{object[1u]};
    auto& p2{object[2u]};

    if (p1.model_pos.y < p0.model_pos.y) std::swap(p0, p1);
    if (p2.model_pos.y < p1.model_pos.y) std::swap(p1, p2);
    if (p1.model_pos.y < p0.model_pos.y) std::swap(p0, p1);

    if (std::fabs(p0.model_pos.y - p1.model_pos.y) < 1e-7f) {
      if (p1.model_pos.x < p0.model_pos.x) std::swap(p0, p1);
      DrawFlatTopTriangle(p0, p1, p2);
    } else if (std::fabs(p1.model_pos.y - p2.model_pos.y) < 1e-7f) {
      if (p2.model_pos.x < p1.model_pos.x) std::swap(p1, p2);
      DrawFlatBottomTriangle(p0, p1, p2);
    } else {
      float const alpha{(p1.model_pos.y - p0.model_pos.y) /
                        (p2.model_pos.y - p0.model_pos.y)};
      auto const pi{interpolate(p0, p2, alpha)};

      if (p1.model_pos.x < pi.model_pos.x) {
        DrawFlatBottomTriangle(p0, p1, pi);
        DrawFlatTopTriangle(p1, pi, p2);
      } else {
        DrawFlatBottomTriangle(p0, pi, p1);
        DrawFlatTopTriangle(pi, p1, p2);
      }
    }
  }

  void DrawFlatTopTriangle(GSV const& p0, GSV const& p1, GSV const& p2) {
    float const delta_y{p2.model_pos.y - p0.model_pos.y};

    auto const left_slope_step{(p2 - p0) / delta_y};
    auto const right_slope_step{(p2 - p1) / delta_y};

    DrawFlatTriangle(p0, p1, p2, left_slope_step, right_slope_step, p1);
  }

  void DrawFlatBottomTriangle(GSV const& p0, GSV const& p1, GSV const& p2) {
    float const delta_y{p2.model_pos.y - p0.model_pos.y};

    auto const left_slope_step{(p1 - p0) / delta_y};
    auto const right_slope_step{(p2 - p0) / delta_y};

    DrawFlatTriangle(p0, p1, p2, left_slope_step, right_slope_step, p0);
  }

  void DrawFlatTriangle(GSV const& p0, GSV const& p1, GSV const& p2,
                        const GSV& dv0, const GSV& dv1, GSV right) {
    auto left{p0};

    int const y_start{std::max((int)std::ceil(p0.model_pos.y - 0.5f), 0)};
    int const y_end{std::min((int)std::ceil(p2.model_pos.y - 0.5f),
                             (int)Graphics::ScreenHeight - 1)};

    left += dv0 * (float(y_start) + 0.5f - p0.model_pos.y);
    right += dv1 * (float(y_start) + 0.5f - p0.model_pos.y);

    for (int y{y_start}; y < y_end; ++y, left += dv0, right += dv1) {
      int const x_start{std::max((int)std::ceil(left.model_pos.x - 0.5f), 0)};
      int const x_end{std::min((int)std::ceil(right.model_pos.x - 0.5f),
                               (int)Graphics::ScreenWidth - 1)};

      auto iLine{left};
      auto const dx{right.model_pos.x - left.model_pos.x};
      auto const diLine{(right - left) / dx};

      iLine += diLine * (float(x_start) + 0.5f - left.model_pos.x);

      for (int x{x_start}; x < x_end; ++x, iLine += diLine) {
        if (UpdateZBuffer({(unsigned)x, (unsigned)y}, iLine.model_pos.z)) {
          float const recovered_w{1.f / iLine.model_pos.w};
          gfx.PutPixel(x, y, effect.ps(iLine * recovered_w));
        }
      }
    }
  }

  void ResetZBuffer() {
    std::fill_n(zbuffer.get()->get(), gfx.ScreenWidth * gfx.ScreenHeight,
                std::numeric_limits<float>::infinity());
  }

  bool UpdateZBuffer(_Vec2<unsigned int> pos, float z) {
    auto zb{zbuffer.get()->get()};
    if (zb[pos.y * gfx.ScreenWidth + pos.x] > z) {
      zb[pos.y * gfx.ScreenWidth + pos.x] = z;
      return true;
    }
    return false;
  }

 private:
  Graphics& gfx;
  float screen_half_width;
  float screen_half_height;

  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;

 public:
  GraphicEffect effect;
};