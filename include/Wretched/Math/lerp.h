#pragma once
#include <math.h>
#include <ostream>

namespace Wretched {
  template <typename T, typename G>
  typename std::enable_if<std::is_arithmetic<G>::value, T>::type Lerp(T a, T b, G c) {
    return a + (b - a) * c;
  };
}

