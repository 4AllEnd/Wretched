#pragma once
#include <deque>
#include <chrono>

namespace Wretched {
  class FramerateManager {
    typedef float time_type;
  public:
    FramerateManager();
    FramerateManager(float);
    void set();
    bool govern();
    void manage_fps();
    double deltatime() const;
    float getfps() const;
    float getavgfps() const;
    float getidealfps() const;
  protected:
    std::chrono::high_resolution_clock::time_point t_past;
    time_type idealfps, maxfps, currentfps, averagefps, delta;
    int ct = 0;
    std::deque<time_type> pastfps;
  };
}
