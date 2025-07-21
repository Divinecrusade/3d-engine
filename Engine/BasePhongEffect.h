#pragma once

#include "Colors.h"
#include "Vec4.h"
#include <cmath>

class BasePhongEffect {
 public:
  BasePhongEffect(Vec3 diffuse = {1.f, 1.f, 1.f},
                  Vec3 ambient = {0.1f, 0.1f, 0.1f})
      : diffuse{diffuse},
        ambient{ambient}
  {}

  template <class V>
  Color Shade(V const& transformed_v, Vec3 const& material,
              float linear_attenuation,
              float quadradic_attenuation,
              float constant_attenuation, float specular_power_factor,
              float specular_range_factor) const {
    auto const surface_n{transformed_v.n.GetNormalized()};
    auto const to_light{light_pos - transformed_v.worldPos};
    auto const distance_to_light{to_light.Len()};
    auto const to_light_n{to_light / distance_to_light};
    auto const attenuation{
        1.f / (quadradic_attenuation * distance_to_light * distance_to_light +
               linear_attenuation * distance_to_light + constant_attenuation)};

    auto const r{surface_n * (to_light * surface_n) * 2.f - to_light};

    Vec3 const speculared{
        diffuse * specular_range_factor *
        std::pow(std::max(0.f, -r.GetNormalized() * transformed_v.worldPos),
                 specular_power_factor)};
    Vec3 const diffused{diffuse * attenuation *
                        std::max(0.f, surface_n * to_light_n)};
    Color const c{
        material.GetHadamarded(diffused + ambient + speculared).GetSaturated() *
        255.f};

    return c;
  }

  void SetLightPosition(Vec4 pos) { light_pos = pos; }

 private:
  Vec3 diffuse;
  Vec3 ambient;

  Vec4 light_pos{};
};