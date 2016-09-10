#include "vec2.h"

namespace Wretched {
  namespace Math {
    template <typename T>
    struct Rect {
      union {
        struct {
          vec2<T> min,
                  max;
        };

        T bounds[4];
      };

      Rect(): min((T)0), max((T)0) {}

      Rect(T a, T b, T c, T d): min(a, b), max(c, d) {}

      Rect(vec2<T> a, T b, T c): min(a.x, a.y), max(b, c) {}

      vec2<T> getcenter() {
        return (min + max) / 2;
      }

      T* data() {
        return *this;
      }

      T& operator[](int i) {
        return bounds[i];
      }
    };
  }
}

