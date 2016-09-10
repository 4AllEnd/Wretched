#pragma once
#include <math.h>
#include <ostream>

namespace Wretched {
  template <typename T = float>
  struct vec3 {
    vec3() : r(0), g(0), b(0) {}

    vec3(T a_, T b_, T c_) : r(a_), g(b_), b(c_) {}

    vec3(T a_) : r(a_), g(a_), b(a_) {}

    vec3 operator+(vec3 a_) {
      return {a_.r + r, a_.g + g, a_.b + b};
    }

    vec3 operator*(T a_) {
      return {a_ * x, a_ * y, a_ * z};
    }

    struct {
      union { T x, r, s; };

      union { T y, g, t; };

      union { T z, b, q; };

      T data[3];
    };
  };
}

