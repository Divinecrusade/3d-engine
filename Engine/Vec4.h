#pragma once

#include "Vec3.h"

template <typename T>
class _Vec4 : public _Vec3<T> {
 public:
  _Vec4() : _Vec3<T>((T)0, (T)0, (T)0), w(1) {}
  _Vec4(T x, T y, T z, T w = (T)1) : _Vec3<T>(x, y, z), w(w) {}
  _Vec4(const _Vec4& vect) : _Vec4(vect.x, vect.y, vect.z, vect.w) {}
  _Vec4(const _Vec3<T>& vect, T w = (T)1) : _Vec3<T>(vect), w(w) {}
  template <typename T2>
  explicit operator _Vec4<T2>() const {
    return {(T2)x, (T2)y, (T2)z, (T2)w};
  }
  _Vec4 operator-() const { return _Vec4(-x, -y, -z); }
  _Vec4& operator=(const _Vec4& rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
  }
  _Vec4& operator+=(const _Vec4& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  _Vec4& operator-=(const _Vec4& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  T operator*(const _Vec4& rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }
  _Vec4 operator%(const _Vec4& rhs) const {
    return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x};
  }
  _Vec4 operator+(const _Vec4& rhs) const { return _Vec4(*this) += rhs; }
  _Vec4 operator-(const _Vec4& rhs) const { return _Vec4(*this) -= rhs; }
  _Vec4& operator*=(const T& rhs) {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }
  _Vec4 operator*(const T& rhs) const { return _Vec4(*this) *= rhs; }
  _Vec4& operator/=(const T& rhs) {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
  }
  _Vec4 operator/(const T& rhs) const { return _Vec4(*this) /= rhs; }
  bool operator==(const _Vec4& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }
  bool operator!=(const _Vec4& rhs) const { return !(*this == rhs); }

 public:
  T w;
};

typedef _Vec4<float> Vec4;
