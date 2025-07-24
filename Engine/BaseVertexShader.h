#pragma once
#include "Mat4.h"

template<class InVertex_, class OutVertex_>
class BaseVertexShader {
 public:
  using InVertex = InVertex_;
  using OutVertex = OutVertex_;

  virtual OutVertex operator()(InVertex const&) = 0;

  void BindWorldView(Mat4 const& new_worldView) {
    worldView = new_worldView;
    worldViewProj = worldView * proj;
  }

  void BindProjection(Mat4 const& new_projection) {
    proj = new_projection;
    worldViewProj = worldView * proj;
  }

  Mat4 const& GetProjection() const { return proj; }

 protected:
  Mat4 worldView{Mat4::Identity()};
  Mat4 proj{Mat4::Identity()};
  Mat4 worldViewProj{Mat4::Identity()};
};