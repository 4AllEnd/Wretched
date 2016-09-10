#pragma once
#include <math.h>
#include <ostream>

namespace Wretched {
  namespace Math {
    template <typename T = float>
    struct vec2 {
      vec2() {}

      vec2(T a_) : r(a_), g(a_) {}

      vec2(T a_, T b_) : r(a_), g(b_) {}

      union {
        struct {
          union { T x, r, s; };

          union { T y, g, t; };
        };

        T _data[2];
      };

      T& operator[](const int i) {
        return _data[i];
      }
    };

    template <typename T>
    vec2<T> operator+(const vec2<T> &a, const vec2<T> &b) {
      return vec2<T>(a.x + b.x, a.y + b.y);
    }

    template <typename T>
    vec2<T> operator+(const vec2<T> &a, const T &b) {
      return vec2<T>(a.x + b, a.y + b);
    }

    template <typename T>
    vec2<T> operator+(const T &a, const vec2<T> &b) {
      return vec2<T>(b.x + a, b.y + a);
    }

    template <typename T>
    vec2<T> operator-(const vec2<T> &a) {
      return vec2<T>(-a.x, -a.y);
    }

    template <typename T>
    vec2<T> operator-(const vec2<T> &a, const vec2<T> &b) {
      return (-b) + a;
    }

    template <typename T>
    vec2<T> operator-(const vec2<T> &a, const T &b) {
      return vec2<T>(a.x - b, a.y - b);
    }

    template <typename T>
    vec2<T> operator-(const T &a, const vec2<T> &b) {
      return a + (-b);
    }

    template <typename T>
    vec2<T> operator*(const vec2<T> &a, const vec2<T> &b) {
      return vec2<T>(a.x * b.x, a.y * b.y);
    }

    template <typename T>
    vec2<T> operator*(const vec2<T> &a, const T &b) {
      return vec2<T>(a.x * b, a.y * b);
    }

    template <typename T>
    vec2<T> operator*(const T &a, const vec2<T> &b) {
      return vec2<T>(a * b.x, a * b.y);
    }

    template <typename T>
    vec2<T> operator/(const vec2<T> &a, const T &b) {
      return vec2<T>(a.x / b, a.y / b);
    }
  }
}

