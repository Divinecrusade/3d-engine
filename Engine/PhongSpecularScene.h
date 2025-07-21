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
    pip_model.effect.vs.BindProjection(projection);
    pip_point_light_dummy.effect.vs.BindProjection(projection);
    pip_model.effect.ps.MoveLight(light_pos);
  }

  void Update(Keyboard& kbd, float dt) {
    if (kbd.KeyIsPressed('W')) {
      camera_pos.z += dt * camera_speed;
    }
    if (kbd.KeyIsPressed('S')) {
      camera_pos.z -= dt * camera_speed;
    }
    if (kbd.KeyIsPressed('A')) {
      camera_pos.x -= dt * camera_speed;
    }
    if (kbd.KeyIsPressed('D')) {
      camera_pos.x += dt * camera_speed;
    }
    if (kbd.KeyIsPressed('Z')) {
      camera_pos.y -= dt * camera_speed;
    }
    if (kbd.KeyIsPressed('X')) {
      camera_pos.y += dt * camera_speed;
    }

    auto const view_offset{-camera_pos};
    pip_model.effect.vs.BindWorldTransformation(
        Mat4::RotationY(PI) * Mat4::Translation(object_pos));
    pip_model.effect.vs.BindView(Mat4::Translation(view_offset));
    //pip_model.effect.ps.MoveLight(view_offset);
    pip_point_light_dummy.effect.vs.BindWorldViewTransformation(
        Mat4::Translation(light_pos) * 
        Mat4::Translation(view_offset));
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
  static constexpr float FOV = 90;
  static constexpr float screen_ratio = 4.f / 3.f;
  static constexpr float _near = 1.f;
  static constexpr float _far = 4.f;
  Mat4 projection{Mat4::PerspectiveProjectionFromFOV(FOV, screen_ratio, _near, _far)};

  static constexpr float camera_speed = 0.5f;
  Vec3 camera_pos{0.f, 0.f, 0.f};

  Vec3 light_pos{0.3f, 0.3f, 1.f};
  Vec3 object_pos{0.0f, 0.0f, 2.5f};
};