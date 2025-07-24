#pragma once

#include <cmath>

#include "BasePhongEffect.h"
#include "BaseVertexShader.h"
#include "DefaultGeometryShader.h"
#include "Mat4.h"

class PhongSpecularEffect {
 public:
  class VertexWithNormal : public Vec4 {
   public:
    VertexWithNormal(Vec3 v) : Vec4{v} {}

    Vec4 GetModel() const { return *this; }

   public:
    Vec3 n{0.f, 0.f, 0.f};
  };

  using Vertex = VertexWithNormal;

  class VertexWithNormalAndWorldPos {
   public:
    VertexWithNormalAndWorldPos operator-(
        VertexWithNormalAndWorldPos const& rhs) const {
      return {model_pos - rhs.model_pos, n - rhs.n, worldPos - rhs.worldPos};
    }

    VertexWithNormalAndWorldPos operator+(
        VertexWithNormalAndWorldPos const& rhs) const {
      return {model_pos + rhs.model_pos, n + rhs.n, worldPos + rhs.worldPos};
    }

    auto operator*(VertexWithNormalAndWorldPos const& rhs) const {
      return model_pos * rhs.model_pos;
    }

    VertexWithNormalAndWorldPos operator%(
        VertexWithNormalAndWorldPos const& rhs) const {
      return {model_pos % rhs.model_pos, n, worldPos};
    }

    auto operator+=(VertexWithNormalAndWorldPos const& rhs) {
      model_pos += rhs.model_pos;
      n += rhs.n;
      worldPos += rhs.worldPos;
      return *this;
    }

    VertexWithNormalAndWorldPos operator*(float factor) const {
      return {model_pos * factor, n * factor, worldPos * factor};
    }

    VertexWithNormalAndWorldPos operator/(float factor) const {
      return {model_pos / factor, n / factor, worldPos / factor};
    }

    auto operator*=(float factor) {
      model_pos *= factor;
      n *= factor;
      worldPos *= factor;
      return *this;
    }

    Vec4 model_pos{};
    Vec3 n{};
    Vec3 worldPos{};
  };

  class VertexShader
      : public BaseVertexShader<Vertex, VertexWithNormalAndWorldPos> {
   public:
    OutVertex operator()(InVertex const& v) override {
      return {v * worldViewProj, Vec4{v.n, 0.f} * worldView, v * worldView};
    }
  };
  using GeometryShader = DefaultGeometryShader<VertexShader::OutVertex>;
  class PixelShader : public BasePhongEffect {
   public:
    PixelShader() : BasePhongEffect{} {}

    Color operator()(GeometryShader::OutVertex const& transformed_v) const {
      return Shade(transformed_v, Vec3{0.8f, 0.85f, 1.f}, linear_attenuation,
          quadradic_attenuation, constant_attenuation, specular_power_factor,
          specular_range_factor);
    }

   private:
    static constexpr float linear_attenuation{0.4f};
    static constexpr float quadradic_attenuation{0.319f};
    static constexpr float constant_attenuation{0.282f};
    
    static constexpr float specular_power_factor{0.7f};
    static constexpr float specular_range_factor{0.2f};
  };

 public:
  PixelShader ps{};
  VertexShader vs{};
  GeometryShader gs{};
};