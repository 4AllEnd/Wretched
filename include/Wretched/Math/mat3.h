#pragma once
#include <math.h>
#include <ostream>

namespace Wretched {
  template <typename T = float>
  struct mat3 {
    vec3<T>& operator[](const int g_) {
      return &m[g_];
    }

  protected:
    vec3<T> m[3];
  };
}

