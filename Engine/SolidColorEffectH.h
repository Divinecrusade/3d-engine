#pragma once

#include "Mat4.h"
#include "DefaultGeometryShader.h"
#include "BaseVertexShader.h"
#include "Colors.h"

class SolidColorEffectH {
 public:
  SolidColorEffectH(Color c)
  { ps.c = c; }

  using Vertex = Vec4;
  class Vertex4 {
   public:
    Vertex4& operator*=(float const& rhs) {
      model_pos *= rhs;
      return *this;
    }
    Vertex4 operator-(Vertex4 const& rhs) const {
      return {model_pos - rhs.model_pos};
    }
    Vertex4 operator+(Vertex4 const& rhs) const {
      return {model_pos + rhs.model_pos};
    }
    Vertex4 operator*(float const& rhs) const {
      return {Vertex4{*this} *= rhs};
    }
    Vertex4 operator/(float const& rhs) const {
      return {Vertex4{*this} *= 1.f / rhs};
    }
    Vertex4& operator+=(Vertex4 const& rhs) {
      model_pos += rhs.model_pos;
      return *this;
    }
    Vec4 model_pos{};
  };

  class VertexShader : public BaseVertexShader<Vertex, Vertex4> {
   public:
    OutVertex operator()(InVertex const& v) {
      return {v * worldViewProj};
    }
  };
  using GeometryShader = DefaultGeometryShader<VertexShader::OutVertex>;
  class PixelShader {
   public:
    Color operator()(GeometryShader::OutVertex const&) const {
      return c;
    }

    Color c;
  };

 public:
  PixelShader ps{};
  VertexShader vs{};
  GeometryShader gs{};
};