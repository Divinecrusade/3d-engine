#pragma once

#include "PhongShadingEffect.h"
#include "IScene.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "SolidColorEffect.h"
#include "Sphere.h"

class PointLightPhongObjectScene : public IScene {
 public:
  PointLightPhongObjectScene(Graphics& gfx, std::string const& filename)
      : PointLightPhongObjectScene{
            gfx,
            IndexedTriangleList<PhongShadingEffect::Vertex>::LoadWithNormals(
                filename)} {}

  PointLightPhongObjectScene(
      Graphics& gfx,
                  IndexedTriangleList<PhongShadingEffect::Vertex> model)
      : zbuffer{std::make_shared<std::unique_ptr<float[]>>(
            std::make_unique<float[]>(gfx.ScreenWidth * gfx.ScreenHeight))},
        pip_model{gfx, PhongShadingEffect{}, zbuffer},
        pip_point_light_dummy{gfx, Colors::White, zbuffer},
        model{std::move(model)},
        point_light_dummy{
            Sphere::GetTriangles<SolidColorEffect::Vertex>(0.2f)} {
    pip_model.effect.vs.SaveTranslation(Vec3{0.0f, 0.0f, 3.f});
    pip_model.effect.vs.SaveRotation(Mat3::RotationY(PI));
    pip_point_light_dummy.effect.vs.SetTranslation(Vec3{1.5f, 1.5f, 3.f});
  }

  void Update(Keyboard& kbd, float dt) {
    if (kbd.KeyIsPressed('W')) {
      pip_point_light_dummy.effect.vs.SaveTranslation(Vec3{0.f, 0.f, 1.f} * dt);
    }
    if (kbd.KeyIsPressed('A')) {
      pip_point_light_dummy.effect.vs.SaveTranslation(Vec3{-1.f, 0.f, 0.f} *
                                                      dt);
    }
    if (kbd.KeyIsPressed('S')) {
      pip_point_light_dummy.effect.vs.SaveTranslation(Vec3{0.f, 0.f, -1.f} *
                                                      dt);
    }
    if (kbd.KeyIsPressed('D')) {
      pip_point_light_dummy.effect.vs.SaveTranslation(Vec3{1.f, 0.f, 0.f} * dt);
    }
    if (kbd.KeyIsPressed('Z')) {
      pip_point_light_dummy.effect.vs.SaveTranslation(Vec3{0.f, -1.f, 0.f} *
                                                      dt);
    }
    if (kbd.KeyIsPressed('X')) {
      pip_point_light_dummy.effect.vs.SaveTranslation(Vec3{0.f, 1.f, 0.f} * dt);
    }
  }
  void Draw() {
    pip_model.BeginFrame();

    auto const translation{pip_point_light_dummy.effect.vs.GetTranslation()};
    pip_point_light_dummy.Draw(point_light_dummy);
    pip_model.effect.ps.MoveLight(translation);
    pip_model.Draw(model);
    pip_model.effect.ps.MoveLight(-translation);
  }

 private:
  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;
  Pipeline<PhongShadingEffect> pip_model;
  Pipeline<SolidColorEffect> pip_point_light_dummy;
  IndexedTriangleList<PhongShadingEffect::Vertex> model;
  IndexedTriangleList<SolidColorEffect::Vertex> point_light_dummy;
  static constexpr float dTheta = PI;
};