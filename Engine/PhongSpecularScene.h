#pragma once

#include "IScene.h"
#include "Pipeline.h"
#include "Sphere.h"
#include "PhongSpecularEffect.h"
#include "SolidColorEffectH.h"

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
        pip_point_light_dummy{gfx, {Colors::White}, zbuffer},
        pip_model{gfx, PhongSpecularEffect{}, zbuffer},
        model{std::move(model)},
        point_light_dummy{Sphere::GetTriangles<SolidColorEffectH::Vertex>(0.1f)}
  {
    pip_model.effect.ps.MoveLight(Vec4{0.3f, 0.3f, 1.f});
    pip_model.effect.vs.BindProjection(projection);
    pip_point_light_dummy.effect.vs.BindWorldTransformation(Mat4::Translation(Vec4{0.8f, 0.3f, 1.5f}));
    pip_point_light_dummy.effect.vs.BindProjection(projection);
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
      zoomFactor += 0.01f;
    }
    if (kbd.KeyIsPressed('X')) {
      zoomFactor -= 0.01f;
    }
    if (kbd.KeyIsPressed('Q')) {
      zTranslation += 0.1f;
    }
    if (kbd.KeyIsPressed('E')) {
      zTranslation -= 0.1f;
    }

    pip_model.effect.vs.BindWorldTransformation(
        Mat4::Scaling(zoomFactor) * Mat4::RotationX(thetaX) *
        Mat4::RotationY(thetaY) * Mat4::RotationZ(thetaZ) *
        Mat4::Translation(Vec4{0.0f, 0.0f, 2.5f + zTranslation}));
  }
  void Draw() {
    pip_model.BeginFrame();

    pip_point_light_dummy.Draw(point_light_dummy);
    pip_model.Draw(model);
  }

 private:
  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;
  Pipeline<PhongSpecularEffect> pip_model;
  Pipeline<SolidColorEffectH> pip_point_light_dummy;
  IndexedTriangleList<PhongSpecularEffect::Vertex> model;
  IndexedTriangleList<SolidColorEffectH::Vertex> point_light_dummy;
  static constexpr float dTheta = PI;
  Mat4 projection{Mat4::PerspectiveProjectionFromFOV(90.f, 4.f / 3.f, 1.f, 4.f)};

  float thetaX{0.f};
  float thetaY{PI};
  float thetaZ{0.f};
  float zoomFactor{1.f};
  float zTranslation{0.f};
};