#pragma once

#include "Mat4.h"
#include "DefaultGeometryShader.h"
#include "Colors.h"

class SolidColorEffectH {
 public:
  SolidColorEffectH(Color c)
  { ps.c = c; }

  using Vertex = Vec4;

  class VertexShader {
   public:
    using InVertex = Vertex;
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
    using OutVertex = Vertex4;

    OutVertex operator()(InVertex const& v) {
      return {v * worldProj};
    }

    void BindWorldTransformation(Mat4 const& new_transformation) {
      world = new_transformation;
      worldProj = world * proj;
    }

    void BindProjection(Mat4 const& new_projection) {
      proj = new_projection;
      worldProj = world * proj;
    }

    Mat4 const& GetProjection() const { return proj; }

   private:
    Mat4 world{Mat4::Identity()};
    Mat4 proj{Mat4::Identity()};
    Mat4 worldProj{Mat4::Identity()};
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