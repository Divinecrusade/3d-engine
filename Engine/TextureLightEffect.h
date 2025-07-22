#pragma once

#include "DefaultGeometryShader.h"
#include "BaseVertexShader.h"
#include "Vec4.h"
#include "Surface.h"

#include <utility>

class TextureLightEffect {
 public:
  class TextureBindedVertex {
   public:
    TextureBindedVertex(Vec3 model = {0.f, 0.f, 0.f})
        : TextureBindedVertex{Vec4{model}} {
    }
    TextureBindedVertex(Vec4 model, Vec2 texture = {0.f, 0.f}, Vec3 n = {0.f, 0.f, 0.f})
        : model_pos{std::move(model)}, texture_pos{std::move(texture)}, n{std::move(n)} {}

    TextureBindedVertex& operator+=(TextureBindedVertex const& rhs) {
      model_pos += rhs.model_pos;
      texture_pos += rhs.texture_pos;
      n += rhs.n;

      return *this;
    }
    TextureBindedVertex& operator-=(TextureBindedVertex const& rhs) {
      model_pos -= rhs.model_pos;
      texture_pos -= rhs.texture_pos;
      n -= rhs.n;

      return *this;
    }

    TextureBindedVertex& operator*=(float const& rhs) {
      model_pos *= rhs;
      texture_pos *= rhs;
      n *= rhs;

      return *this;
    }
    TextureBindedVertex& operator/=(float const& rhs) {
      model_pos /= rhs;
      texture_pos /= rhs;
      n /= rhs;

      return *this;
    }

    TextureBindedVertex operator+(TextureBindedVertex const& rhs) const {
      return TextureBindedVertex{*this} += rhs;
    }
    TextureBindedVertex operator-(TextureBindedVertex const& rhs) const {
      return TextureBindedVertex{*this} -= rhs;
    }

    TextureBindedVertex operator*(float const& rhs) const {
      return TextureBindedVertex{*this} *= rhs;
    }
    TextureBindedVertex operator/(float const& rhs) const {
      return TextureBindedVertex{*this} /= rhs;
    }
    float operator*(TextureBindedVertex const& rhs) const {
      return this->model_pos * rhs.model_pos;
    }

    Vec4 model_pos{};
    Vec2 texture_pos{};
    Vec3 n{};
  };

  using Vertex = TextureBindedVertex;

  class TBDVwithWorldPos : public TextureBindedVertex {
    public:
      TBDVwithWorldPos(Vec4 model, Vec2 texture, Vec3 n, Vec4 world_pos)
        : TextureBindedVertex{std::move(model), std::move(texture), std::move(n)},
          world_pos{std::move(world_pos)}
          { }


      TBDVwithWorldPos operator+(TBDVwithWorldPos const& rhs) const {
        return TBDVwithWorldPos{model_pos + rhs.model_pos,
                                texture_pos + rhs.texture_pos, 
                                n + rhs.n,
                                world_pos + rhs.world_pos};
      }
      TBDVwithWorldPos operator-(TBDVwithWorldPos const& rhs) const {
        return TBDVwithWorldPos{model_pos - rhs.model_pos,
                                texture_pos - rhs.texture_pos, 
                                n - rhs.n,
                                world_pos - rhs.world_pos};
      }
      TBDVwithWorldPos operator*(float const& rhs) const {
        return TBDVwithWorldPos{model_pos * rhs,
                                texture_pos * rhs, 
                                n * rhs,
                                world_pos * rhs};
      }

      TBDVwithWorldPos operator/(float const& rhs) const {
        return TBDVwithWorldPos{model_pos / rhs, texture_pos / rhs, n / rhs,
                                world_pos / rhs};
      }

    public:
      Vec4 world_pos; 
  };

  class VertexShader : public BaseVertexShader<TextureBindedVertex, TBDVwithWorldPos> {
   public:
    OutVertex operator()(InVertex const& v) {
      return {v.model_pos * worldViewProj, v.texture_pos, Vec4{v.n, 0.f} * worldView, v.model_pos * worldView};
    }
  };

  using GeometryShader = DefaultGeometryShader<TBDVwithWorldPos>;

  class PixelShader {
   public:
    PixelShader(Surface&& texture, Vec3 diffuse,
                Vec3 ambient,
                float linear_attenuation, float quadradic_attenuation,
                float constant_attenuation)
        : texture{std::forward<Surface>(texture)}, diffuse{std::move(diffuse)}, ambient{std::move(ambient)},
        linear_attenuation{linear_attenuation}, quadradic_attenuation{quadradic_attenuation}, 
        constant_attenuation{constant_attenuation}
    {}

    using InVertex = GeometryShader::OutVertex;

    Color operator()(InVertex const& v) const {
      auto const texture_width{texture.GetWidth()};
      auto const texture_height{texture.GetHeight()};
      unsigned int const x{(unsigned)(v.texture_pos.x * (float)texture_width + 0.5f)};
      unsigned int const y{(unsigned)(v.texture_pos.y * (float)texture_height + 0.5f)};
      
      Color const texture_c{texture.GetPixel(x % texture_width, y % texture_height)}; 
      Vec3 material{(float)texture_c.GetR() / 255.f,
                    (float)texture_c.GetG() / 255.f,
                    (float)texture_c.GetB() / 255.f};
      auto const to_light{light_pos - v.world_pos};
      auto const distance_to_light{to_light.Len()};
      auto const to_light_n{to_light / distance_to_light};
      auto const attenuation{
          1.f /
          (quadradic_attenuation * distance_to_light * distance_to_light +
           linear_attenuation * distance_to_light + constant_attenuation)};

      Vec3 const diffused{
          diffuse * attenuation *
          std::max(0.f, v.n.GetNormalized() * to_light_n)};
      Color const c{material.GetHadamarded(diffused + ambient).GetSaturated() * 255.f};

      return c;
    }

    void SetLightPosition(Vec4 pos) { light_pos = pos; }

   private:
    Surface texture;
    
    Vec3 diffuse;
    Vec3 ambient;

    float linear_attenuation;
    float quadradic_attenuation;
    float constant_attenuation;

    Vec4 light_pos{};
  };

public:
  TextureLightEffect(std::wstring const& texture_url,
                     Vec3 diffuse = {1.f, 1.f, 1.f},
                     Vec3 ambient = {0.1f, 0.1f, 0.1f},
                     float linear_attenuation = 2.6f,
                     float quadradic_attenuation = 3.f,
                     float constant_attenuation = 1.3f)
      : ps{Surface::FromFile(texture_url),
           std::move(diffuse),
           std::move(ambient),
           linear_attenuation,
           quadradic_attenuation,
           constant_attenuation} 
  {}

  VertexShader vs{};
  GeometryShader gs{};
  PixelShader ps;
};