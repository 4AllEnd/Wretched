#pragma once
#include <math.h>
#include <ostream>

namespace Wretched {
  template <typename T = float>
  struct mat4 {
    vec4<T>& operator[](const int g_) {
      return &m[g_];
    }

  protected:
    vec4<T> m[4];
  };
}

