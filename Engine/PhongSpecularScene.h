#pragma once

#include "IScene.h"
#include "Pipeline.h"
#include "Sphere.h"
#include "PhongSpecularEffect.h"
#include "SolidColorEffectH.h"
#include "ChiliMath.h"

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
  }

  void Update(Keyboard& kbd, float dt, Mouse& mouse) {
    while (!mouse.IsEmpty()) {
      auto const e{mouse.Read()};
      switch (e.GetType()) {
        case Mouse::Event::Type::LPress:
          mouse_engaged = true;
          mouse_pos = e.GetPos();
          break;

        case Mouse::Event::Type::LRelease:
          mouse_engaged = false;
          break;

        case Mouse::Event::Type::Move:
          if (!mouse_engaged) break;

          auto const delta_pos{e.GetPos() - mouse_pos};
          camera_rot_inv = camera_rot_inv *
                       Mat4::RotationX(w_angle_delta * float(delta_pos.y)) *
                       Mat4::RotationY(h_angle_delta * float(delta_pos.x));
          mouse_pos = e.GetPos();
          break;
      }
    }
    
    if (kbd.KeyIsPressed('W')) {
      camera_pos += Vec4{0.f, 0.f, 1.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('S')) {
      camera_pos -= Vec4{0.f, 0.f, 1.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('A')) {
      camera_pos -= Vec4{1.f, 0.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('D')) {
      camera_pos += Vec4{1.f, 0.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('Z')) {
      camera_pos = Vec4{0.f, 1.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('X')) {
      camera_pos = Vec4{0.f, 1.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }

    auto const view_offset{-camera_pos};
    pip_model.effect.vs.BindWorldView(worldForObject, 
        Mat4::Translation(view_offset) * camera_rot_inv);
    pip_model.effect.ps.SetLightPosition(light_pos + Vec4{view_offset} * camera_rot_inv);
    
    pip_point_light_dummy.effect.vs.BindWorldView( 
        Mat4::Translation(light_pos), 
        Mat4::Translation(view_offset) *
        camera_rot_inv);
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
  static constexpr float wFOV = 90.f;
  static constexpr float screen_ratio = 4.f / 3.f;
  static constexpr float hFOV = wFOV / screen_ratio;
  static constexpr float _near = 1.f;
  static constexpr float _far = 4.f;
  Mat4 projection{Mat4::PerspectiveProjectionFromFOV(wFOV, screen_ratio, _near, _far)};

  static constexpr float h_angle_delta{hFOV / Graphics::ScreenHeight * PI / 180.f};
  static constexpr float w_angle_delta{wFOV / Graphics::ScreenWidth * PI / 180.f};
  Mat4 camera_rot_inv{Mat4::Identity()};

  static constexpr float camera_speed = 0.5f;
  Vec3 camera_pos{0.f, 0.f, 0.f};

  Vec3 light_pos{0.3f, 0.3f, 1.f};
  Vec3 object_pos{0.0f, 0.0f, 2.5f};

  Vei2 mouse_pos{};
  bool mouse_engaged{false};

  Mat4 worldForObject{Mat4::RotationY(PI) * Mat4::Translation(object_pos)};
};