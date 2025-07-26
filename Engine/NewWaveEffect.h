#pragma once

#include "TextureLightEffect.h"

class NewWaveEffect {
 public:

  using Vertex = TextureLightEffect::TextureBindedVertex;

  class VertexShader
      : public BaseVertexShader<Vertex,
                                TextureLightEffect::TextureLightBindedVertex> {
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
      float const angle{wrap_angle(v.model_pos.x * wave_shift_factor + time * wave_length_factor)};
      float const cosx{std::cosf(angle)};
      float const sinx{std::sinf(angle)};
      float const dz{wave_amplitude_factor * cosx};
      Vec4 const shifted_pos{v.model_pos.x, v.model_pos.y, v.model_pos.z + dz};
      Vec3 shifted_n{-wave_shift_factor * wave_amplitude_factor * sinx, 0.f, -1.f};
      shifted_n.Normalize();

      auto const world_pos{shifted_pos * worldView};
      auto const to_light{light_pos - world_pos};
      auto const distance_to_light{to_light.Len()};
      auto const to_light_n{to_light / distance_to_light};
      auto const attenuation{
          1.f /
          (quadradic_attenuation * distance_to_light * distance_to_light +
           linear_attenuation * distance_to_light + constant_attenuation)};

      Vec3 const diffused{
          diffuse * attenuation *
          std::max(0.f, Vec4{shifted_n, 0.f} * worldView * to_light_n)};
      return {world_pos * proj, v.texture_pos, diffused + ambient};
    }

    void SetLightPosition(Vec4 pos) { light_pos = pos; }

    void UpdateTime(float delta_time) {
      time = std::fmodf(time + delta_time, std::numeric_limits<float>::max());
    }

   private:
    Vec3 diffuse;
    Vec3 ambient;
    Vec4 light_pos{};

    float linear_attenuation;
    float quadradic_attenuation;
    float constant_attenuation;

    float time{0.f};
    static constexpr float wave_length_factor{PI};
    static constexpr float wave_shift_factor{45.f};
    static constexpr float wave_amplitude_factor{0.02f};
  };

  using GeometryShader = DefaultGeometryShader<VertexShader::OutVertex>;
  using PixelShader = TextureLightEffect::PixelShader;

 public:
  NewWaveEffect(Vec3 diffuse = {1.f, 1.f, 1.f},
                     Vec3 ambient = {0.1f, 0.1f, 0.1f},
                     float linear_attenuation = 0.5f,
                     float quadradic_attenuation = 0.2f,
                     float constant_attenuation = 0.4f)
      : vs{std::move(diffuse), std::move(ambient), linear_attenuation,
           quadradic_attenuation, constant_attenuation} {}

  VertexShader vs;
  GeometryShader gs{};
  PixelShader ps;
};