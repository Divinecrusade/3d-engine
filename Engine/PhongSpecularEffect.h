#pragma once

#include "DefaultGeometryShader.h"
#include "Mat4.h"
#include <cmath>

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

  class VertexShader {
   public:
    using InVertex = Vertex;

    class VertexWithNormalAndWorldPos {
     public:
      VertexWithNormalAndWorldPos operator-(VertexWithNormalAndWorldPos const& rhs) const {
        return {model_pos - rhs.model_pos, n - rhs.n, worldPos - rhs.worldPos};
      }

      VertexWithNormalAndWorldPos operator+(VertexWithNormalAndWorldPos const& rhs) const {
        return {model_pos + rhs.model_pos, n + rhs.n, worldPos + rhs.worldPos};
      }

      auto operator*(VertexWithNormalAndWorldPos const& rhs) const {
        return model_pos * rhs.model_pos;
      }

      VertexWithNormalAndWorldPos operator%(VertexWithNormalAndWorldPos const& rhs) const {
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

    using OutVertex = VertexWithNormalAndWorldPos;

    OutVertex operator()(InVertex const& v) {
      return {v * worldViewProj, Vec4{v.n, 0.f} * worldView, v * worldView};
    }

    void BindWorldTransformation(Mat4 const& new_transformation) {
      world = new_transformation;
      worldView = world * view;
      worldViewProj = worldView * proj;
    }

    void BindView(Mat4 const& new_view) {
      view = new_view;
      worldView = world * view;
      worldViewProj = worldView * proj;
    }

    void BindProjection(Mat4 const& new_projection) {
      proj = new_projection;
      worldViewProj = worldView * proj;
    }

    Mat4 const& GetProjection() const {
      return proj;
    }

   private:

    Mat4 world{Mat4::Identity()};
    Mat4 view{Mat4::Identity()};
    Mat4 proj{Mat4::Identity()};
    Mat4 worldView{Mat4::Identity()};
    Mat4 worldViewProj{Mat4::Identity()};
  };
  using GeometryShader = DefaultGeometryShader<VertexShader::OutVertex>;
  class PixelShader {
   public:
    Color operator()(GeometryShader::OutVertex const& transformed_v) const {
      auto const surface_n{transformed_v.n.GetNormalized()};
      auto const to_light{light_pos - transformed_v.worldPos};
      auto const distance_to_light{to_light.Len()};
      auto const to_light_n{to_light / distance_to_light};
      auto const attenuation{
          1.f /
          (quadradic_attenuation * distance_to_light * distance_to_light +
           linear_attenuation * distance_to_light + constant_attenuation)};

      auto const r{ surface_n * (to_light * surface_n) * 2.f - to_light};

      Vec3 const speculared{diffuse * specular_range_factor *
                            std::pow(std::max(0.f, -r.GetNormalized() * transformed_v.worldPos), specular_power_factor)};
      Vec3 const diffused{diffuse * attenuation *
                          std::max(0.f, surface_n * to_light_n)};
      Color const c{material.GetHadamarded(diffused + ambient + speculared)
                            .GetSaturated() * 255.f};

      return c;
    }

    void SetLightPosition(Vec4 pos) { light_pos = pos; }

   private:
    Vec3 diffuse{1.f, 1.f, 1.f};
    Vec3 ambient{0.1f, 0.1f, 0.1f};
    Vec3 material{0.8f, 0.85f, 1.f};

    float linear_attenuation{1.0f};
    float quadradic_attenuation{2.619f};
    float constant_attenuation{0.382f};

    float specular_power_factor{5.f};
    float specular_range_factor{0.05f};

    Vec4 light_pos{};
  };

 public:
  PixelShader ps{};
  VertexShader vs{};
  GeometryShader gs{};
};