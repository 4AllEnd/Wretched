#pragma once
#include <math.h>
#include <ostream>

namespace Wretched {
  template <typename T = float>
  struct mat2 {
    vec2<T>& operator[](const int g_) {
      return &m[g_];
    }

  protected:
    vec2<T> m[2];
  };
}

