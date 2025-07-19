#pragma once

#include "IScene.h"
#include "Pipeline.h"
#include "SolidColorEffect.h"
#include "Sphere.h"
#include "PhongSpecularEffect.h"

class PhongSpecularScene : public IScene {
 public:
  PhongSpecularScene(Graphics& gfx, std::string const& filename)
      : PhongSpecularScene{
            gfx,
            IndexedTriangleList<PhongSpecularEffect::Vertex>::LoadWithNormals(
                filename)} {}

  PhongSpecularScene(Graphics& gfx,
                     IndexedTriangleList<PhongSpecularEffect::Vertex> model)
      : zbuffer{std::make_shared<std::unique_ptr<float[]>>(
            std::make_unique<float[]>(gfx.ScreenWidth * gfx.ScreenHeight))},
        pip_model{gfx, PhongSpecularEffect{}, zbuffer},
        pip_point_light_dummy{gfx, Colors::White, zbuffer},
        model{std::move(model)},
        point_light_dummy{
            Sphere::GetTriangles<SolidColorEffect::Vertex>(0.2f)} {
    pip_model.effect.ps.MoveLight(Vec4{0.3f, 0.3f, 1.f});
    pip_point_light_dummy.effect.vs.SetTranslation(Vec3{0.3f, 0.3f, 1.f});
  }

  void Update(Keyboard& kbd, float dt) {
    if (kbd.KeyIsPressed('W')) {
      thetaX += PI / 90.f;
    }
    if (kbd.KeyIsPressed('A')) {
      thetaZ += PI / 90.f;
    }
    if (kbd.KeyIsPressed('S')) {
      thetaX += -PI / 90.f;
    }
    if (kbd.KeyIsPressed('D')) {
      thetaZ += -PI / 90.f;
    }
    if (kbd.KeyIsPressed('Z')) {
      thetaY += PI / 90.f;
    }
    if (kbd.KeyIsPressed('X')) {
      thetaY += -PI / 90.f;
    }

    pip_model.effect.vs.BindTransformation(Mat4::RotationX(thetaX) * Mat4::RotationY(thetaY) * Mat4::RotationZ(thetaZ) * Mat4::Translation(Vec4{0.0f, 0.0f, 3.f}));
  }
  void Draw() {
    pip_model.BeginFrame();

    pip_point_light_dummy.Draw(point_light_dummy);
    pip_model.Draw(model);
  }

 private:
  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;
  Pipeline<PhongSpecularEffect> pip_model;
  Pipeline<SolidColorEffect> pip_point_light_dummy;
  IndexedTriangleList<PhongSpecularEffect::Vertex> model;
  IndexedTriangleList<SolidColorEffect::Vertex> point_light_dummy;
  static constexpr float dTheta = PI;

  float thetaX{0.f};
  float thetaY{PI};
  float thetaZ{0.f};
};