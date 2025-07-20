#pragma once

#include "Vec4.h"

template <typename T>
class _Mat4 {
 public:
  _Mat4& operator=(const _Mat4& rhs) {
    memcpy(elements, rhs.elements, sizeof(elements));
    return *this;
  }
  _Mat4& operator*=(T rhs) {
    for (auto& row : elements) {
      for (T& e : row) {
        e *= rhs;
      }
    }
    return *this;
  }
  _Mat4 operator*(T rhs) const {
    _Mat4 result = *this;
    return result *= rhs;
  }
  _Mat4 operator*(const _Mat4& rhs) const {
    _Mat4 result;
    for (size_t j = 0; j < 4; j++) {
      for (size_t k = 0; k < 4; k++) {
        T sum = (T)0;
        for (size_t i = 0; i < 4; i++) {
          sum += elements[j][i] * rhs.elements[i][k];
        }
        result.elements[j][k] = sum;
      }
    }
    return result;
  }
  _Mat4& operator*=(const _Mat4& rhs) {
    *this = *this * rhs;
    return *this;
  }
  static _Mat4 Identity() {
    return {(T)1, (T)0, (T)0, (T)0, 
            (T)0, (T)1, (T)0, (T)0,
            (T)0, (T)0, (T)1, (T)0, 
            (T)0, (T)0, (T)0, (T)1};
  }
  static _Mat4 Scaling(T factor) {
    return {factor, (T)0, (T)0,   (T)0, 
            (T)0, factor, (T)0, (T)0,
            (T)0, (T)0, factor, (T)0,
            (T)0, (T)0, (T)0,   (T)1};
  }
  static _Mat4 RotationX(T angle) {
    auto const cosTheta{cos(angle)};
    auto const sinTheta{sin(angle)};
    return {(T)1, (T)0, (T)0, (T)0,      
            (T)0, cosTheta, sinTheta, (T)0,
            (T)0, -sinTheta, cosTheta, (T)0,
            (T)0, (T)0, (T)0, (T)1};
  }
  static _Mat4 RotationY(T angle) {
    auto const cosTheta{cos(angle)};
    auto const sinTheta{sin(angle)};
    return {cosTheta, (T)0, -sinTheta, (T)0, 
            (T)0, (T)1, (T)0, (T)0,
            sinTheta, (T)0,  cosTheta, (T)0,
            (T)0, (T)0, (T)0, (T)1};
  }
  static _Mat4 RotationZ(T angle) {
    auto const cosTheta{cosf(angle)};
    auto const sinTheta{sinf(angle)};
    return {cosTheta, sinTheta, (T)0, (T)0,
            -sinTheta, cosTheta, (T)0, (T)0,
            (T)0, (T)0, (T)1, (T)0,
            (T)0, (T)0, (T)0, (T)1};
  }
  static _Mat4 Translation(_Vec4<T> const& by) {
    return {(T)1, (T)0, (T)0, (T)0, 
            (T)0, (T)1, (T)0, (T)0,
            (T)0, (T)0, (T)1, (T)0, 
            by.x, by.y, by.z, (T)1};
  }

  static _Mat4 PerspectiveProjection(T w, T h, T n, T f) {
    return {2 * n / w, (T)0, (T)0, (T)0,
           (T)0, 2 * n / h, (T)0, (T)0,
           (T)0, (T)0, f / (f - n), (T)1,
           (T)0, (T)0, -f * n / (f - n), (T)0};
  }

  static _Mat4 PerspectiveProjectionFromFOV(T fov, T ar, T n, T f) {
    auto const fov_rad{fov * (T)PI / 180};
    auto const w{(T)1.0f / std::tan(fov_rad / (T)2)};
    auto const h{w * ar};

    return { w, (T)0, (T)0, (T)0, 
            (T)0, h, (T)0, (T)0,
            (T)0, (T)0, f / (f - n), (T)1, 
            (T)0, (T)0, -f * n / (f - n), (T)0};
  }

 public:
  // [ row ][ col ]
  T elements[4][4];
};

template <typename T>
_Vec4<T>& operator*=(_Vec4<T>& lhs, const _Mat4<T>& rhs) {
  return lhs = lhs * rhs;
}

template <typename T>
_Vec4<T> operator*(const _Vec4<T>& lhs, const _Mat4<T>& rhs) {
  return {
      lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] +
          lhs.z * rhs.elements[2][0] + lhs.w * rhs.elements[3][0],
      lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] +
          lhs.z * rhs.elements[2][1] + lhs.w * rhs.elements[3][1],
      lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] +
          lhs.z * rhs.elements[2][2] + lhs.w * rhs.elements[3][2],
      lhs.x * rhs.elements[0][3] + lhs.y * rhs.elements[1][3] +
          lhs.z * rhs.elements[2][3] + lhs.w * rhs.elements[3][3],
  };
}

typedef _Mat4<float> Mat4;
typedef _Mat4<double> Mad4;