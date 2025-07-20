#pragma once

#include "IScene.h"
#include "Pipeline.h"
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
        model{std::move(model)}
  {
    pip_model.effect.ps.MoveLight(Vec4{0.3f, 0.3f, 1.f});
    pip_model.effect.vs.BindProjection(projection);
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

    pip_model.effect.vs.BindWorldTransformation(Mat4::RotationX(thetaX) * Mat4::RotationY(thetaY) * Mat4::RotationZ(thetaZ) * Mat4::Translation(Vec4{0.0f, 0.0f, 2.5f}));
  }
  void Draw() {
    pip_model.BeginFrame();

    pip_model.Draw(model);
  }

 private:
  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;
  Pipeline<PhongSpecularEffect> pip_model;
  IndexedTriangleList<PhongSpecularEffect::Vertex> model;
  static constexpr float dTheta = PI;
  Mat4 projection{Mat4::PerspectiveProjectionFromFOV(90.f, 4.f / 3.f, 1.f, 10.f)};

  float thetaX{0.f};
  float thetaY{PI};
  float thetaZ{0.f};
};