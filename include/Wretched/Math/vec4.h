#pragma once
#include <math.h>
#include <ostream>

namespace Wretched {
  template <typename T = float>
  struct vec4 {
    vec4() : r(0), g(0), b(0), a(0) {}

    vec4(T a_, T b_, T c_, T d_) : r(a_), g(b_), b(c_), a(d_) {}

    vec4(T a_) : r(a_), g(a_), b(a_), a(a_) {}

    vec4 operator+(vec4 a_) {
      return {a_.r + r, a_.g + g, a_.b + b,a_.a + a};
    }

    vec4 operator*(T a_) {
      return {a_ * x, a_ * y, a_ * z};
    }

    struct {
      union { T x, r, s; };

      union { T y, g, t; };

      union { T z, b, q; };

      union { T w, a, u; };

      T _data[4];
    }
  };
}

