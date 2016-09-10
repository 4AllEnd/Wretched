#pragma once
#include <chrono>

namespace Wretched {
  struct Timer {
  protected:
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
  public:
    void reset() {
      begin = std::chrono::high_resolution_clock::now();
    };

    double getelapsedtime() {
      return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - begin).count();
    }
  };
};

