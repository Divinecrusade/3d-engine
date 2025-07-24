#pragma once

#include <utility>

#include "BaseVertexShader.h"
#include "DefaultGeometryShader.h"
#include "Surface.h"
#include "Vec4.h"

class TextureLightEffect {
 public:
  class TextureBindedVertex {
   public:
    TextureBindedVertex(Vec3 model = {0.f, 0.f, 0.f})
        : TextureBindedVertex{Vec4{model}} {}
    TextureBindedVertex(Vec4 model, Vec2 texture = {0.f, 0.f},
                        Vec3 n = {0.f, 0.f, 0.f})
        : model_pos{std::move(model)},
          texture_pos{std::move(texture)},
          n{std::move(n)} {}

    Vec4 model_pos{};
    Vec2 texture_pos{};
    Vec3 n{};
  };

  using Vertex = TextureBindedVertex;

  class TextureLightBindedVertex {
   public:
    TextureLightBindedVertex(Vec4 model_pos, Vec2 texture_pos,
                             Vec3 light_component)
        : model_pos{std::move(model_pos)},
          texture_pos{texture_pos},
          light_component{std::move(light_component)} {}

    TextureLightBindedVertex& operator+=(TextureLightBindedVertex const& rhs) {
      model_pos += rhs.model_pos;
      texture_pos += rhs.texture_pos;
      light_component += rhs.light_component;
      return *this;
    }

    TextureLightBindedVertex& operator-=(TextureLightBindedVertex const& rhs) {
      model_pos -= rhs.model_pos;
      texture_pos -= rhs.texture_pos;
      light_component -= rhs.light_component;
      return *this;
    }

    TextureLightBindedVertex& operator*=(float const& rhs) {
      model_pos *= rhs;
      texture_pos *= rhs;
      light_component *= rhs;
      return *this;
    }

    TextureLightBindedVertex& operator/=(float const& rhs) {
      model_pos /= rhs;
      texture_pos /= rhs;
      light_component /= rhs;
      return *this;
    }

    TextureLightBindedVertex operator+(TextureLightBindedVertex const& rhs) const {
      return TextureLightBindedVertex{*this} += rhs;
    }

    TextureLightBindedVertex operator-(TextureLightBindedVertex const& rhs) const {
      return TextureLightBindedVertex{*this} -= rhs;
    }

    TextureLightBindedVertex operator*(float const& rhs) const {
      return TextureLightBindedVertex{*this} *= rhs;
    }

    TextureLightBindedVertex operator/(float const& rhs) const {
      return TextureLightBindedVertex{*this} /= rhs;
    }

   public:
    Vec4 model_pos{};
    Vec2 texture_pos{};
    Vec3 light_component{};
  };

  class VertexShader : public BaseVertexShader<TextureBindedVertex, TextureLightBindedVertex> {
   public:
    VertexShader(Vec3 diffuse, Vec3 ambient, float linear_attenuation,
                 float quadradic_attenuation, float constant_attenuation)
        : BaseVertexShader{},
          diffuse{std::move(diffuse)},
          ambient{std::move(ambient)},
          linear_attenuation{linear_attenuation},
          quadradic_attenuation{quadradic_attenuation},
          constant_attenuation{constant_attenuation} {}

    OutVertex operator()(InVertex const& v) {
      auto const world_pos{v.model_pos * worldView};
      auto const to_light{light_pos - world_pos};
      auto const distance_to_light{to_light.Len()};
      auto const to_light_n{to_light / distance_to_light};
      auto const attenuation{
          1.f /
          (quadradic_attenuation * distance_to_light * distance_to_light +
           linear_attenuation * distance_to_light + constant_attenuation)};

      Vec3 const diffused{diffuse * attenuation * std::max(0.f, static_cast<Vec3>(Vec4{-v.n, 0.f} * worldView) * to_light_n)};
      return {v.model_pos * worldViewProj,
              v.texture_pos,
              diffused + ambient};
    }

    void SetLightPosition(Vec4 pos) { light_pos = pos; }

   private:
    Vec3 diffuse;
    Vec3 ambient;
    Vec4 light_pos{};

    float linear_attenuation;
    float quadradic_attenuation;
    float constant_attenuation;
  };

  using GeometryShader = DefaultGeometryShader<TextureLightBindedVertex>;

  class PixelShader {
   public:
    PixelShader() {}

    using InVertex = GeometryShader::OutVertex;

    void BindTexture(Surface const& new_texture) {
      texture = &new_texture;
    }

    Color operator()(InVertex const& v) const {
      assert(!texture);
      auto const texture_width{texture->GetWidth()};
      auto const texture_height{texture->GetHeight()};
      unsigned int const x{
          (unsigned)(v.texture_pos.x * (float)texture_width + 0.5f)};
      unsigned int const y{
          (unsigned)(v.texture_pos.y * (float)texture_height + 0.5f)};

      Color const texture_c{
          texture->GetPixel(x % texture_width, y % texture_height)};

      Vec3 material{(float)texture_c.GetR() / 255.f,
                    (float)texture_c.GetG() / 255.f,
                    (float)texture_c.GetB() / 255.f};
      Color const c{material.GetHadamarded(v.light_component).GetSaturated() *
                    255.f};

      return c;
    }

   private:
    Surface const* texture{nullptr};
  };

 public:
  TextureLightEffect(Vec3 diffuse = {1.f, 1.f, 1.f},
                     Vec3 ambient = {0.1f, 0.1f, 0.1f},
                     float linear_attenuation = 0.5f,
                     float quadradic_attenuation = 0.2f,
                     float constant_attenuation = 0.4f)
      : vs{std::move(diffuse),
           std::move(ambient),
           linear_attenuation,
           quadradic_attenuation,
           constant_attenuation}
    {}

  VertexShader vs;
  GeometryShader gs{};
  PixelShader ps;
};