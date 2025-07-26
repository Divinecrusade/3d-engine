#pragma once

#include "TextureLightEffect.h"
#include "PhongSpecularEffect.h"

class NewWaveEffect {
 public:

  using Vertex = TextureLightEffect::TextureBindedVertex;

  class TextureBindedVertexWithWorldPos {
   public:
    TextureBindedVertexWithWorldPos operator-(
        TextureBindedVertexWithWorldPos const& rhs) const {
      return {model_pos - rhs.model_pos, n - rhs.n, worldPos - rhs.worldPos, texture_pos - rhs.texture_pos};
    }

    TextureBindedVertexWithWorldPos operator+(
        TextureBindedVertexWithWorldPos const& rhs) const {
      return {model_pos + rhs.model_pos, n + rhs.n, worldPos + rhs.worldPos, texture_pos + rhs.texture_pos};
    }

    auto operator*(TextureBindedVertexWithWorldPos const& rhs) const {
      return model_pos * rhs.model_pos;
    }

    TextureBindedVertexWithWorldPos operator%(
        TextureBindedVertexWithWorldPos const& rhs) const {
      return {model_pos % rhs.model_pos, n, worldPos, texture_pos};
    }

    auto operator+=(TextureBindedVertexWithWorldPos const& rhs) {
      model_pos += rhs.model_pos;
      n += rhs.n;
      worldPos += rhs.worldPos;
      texture_pos += rhs.texture_pos;
      return *this;
    }

    TextureBindedVertexWithWorldPos operator*(float factor) const {
      return {model_pos * factor, n * factor, worldPos * factor, texture_pos * factor};
    }

    TextureBindedVertexWithWorldPos operator/(float factor) const {
      return {model_pos / factor, n / factor, worldPos / factor, texture_pos / factor};
    }

    auto operator*=(float factor) {
      model_pos *= factor;
      n *= factor;
      worldPos *= factor;
      texture_pos *= factor;
      return *this;
    }

    Vec4 model_pos{};
    Vec3 n{};
    Vec3 worldPos{};
    Vec2 texture_pos{};
  };

  class VertexShader : public BaseVertexShader<Vertex, TextureBindedVertexWithWorldPos> {
   public:
    VertexShader() : BaseVertexShader{}
    {}

    OutVertex operator()(InVertex const& v) {
      float const angle{wrap_angle(v.model_pos.x * wave_shift_factor + time * wave_length_factor)};
      float const cosx{std::cosf(angle)};
      float const sinx{std::sinf(angle)};
      float const dz{wave_amplitude_factor * cosx};
      Vec4 const shifted_pos{v.model_pos.x, v.model_pos.y, v.model_pos.z + dz};
      Vec4 shifted_n{-wave_shift_factor * wave_amplitude_factor * sinx, 0.f, -1.f, 0.f};
      shifted_n.Normalize();

      return {shifted_pos * worldViewProj, static_cast<Vec3>(shifted_n * worldView), v.model_pos * worldView, v.texture_pos};
    }

    void UpdateTime(float delta_time) {
      time = std::fmodf(time + delta_time, std::numeric_limits<float>::max());
    }

   private:
    float time{0.f};
    static constexpr float wave_length_factor{PI};
    static constexpr float wave_shift_factor{45.f};
    static constexpr float wave_amplitude_factor{0.02f};
  };

  using GeometryShader = DefaultGeometryShader<VertexShader::OutVertex>;
  class PixelShader : public BasePhongEffect {
   public:
    using InVertex = GeometryShader::OutVertex;
    
    PixelShader() : BasePhongEffect{} {}

    void BindTexture(Surface const& new_texture) { texture = &new_texture; }

    Color operator()(GeometryShader::OutVertex const& v) const {
      assert(texture);
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

      return Shade(v, material, linear_attenuation,
                   quadradic_attenuation, constant_attenuation,
                   specular_power_factor, specular_range_factor);
    }

  private:
    Surface const* texture{nullptr};

   private:
    static constexpr float linear_attenuation{0.2f};
    static constexpr float quadradic_attenuation{0.119f};
    static constexpr float constant_attenuation{0.2f};

    static constexpr float specular_power_factor{0.2f};
    static constexpr float specular_range_factor{0.1f};
  };

 public:
 
  VertexShader vs{};
  GeometryShader gs{};
  PixelShader ps{};
};