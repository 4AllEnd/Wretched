#include <Wretched/Graphics/Framerate.h>

namespace Wretched {
  FramerateManager::FramerateManager() {};


  void FramerateManager::set() {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    delta = std::chrono::duration<time_type>(now - t_past).count();
    t_past = now;
    currentfps = 1.f / delta;
    if (pastfps.size() > 99) {
      pastfps.pop_front();
    }
    pastfps.push_back(currentfps);
    averagefps = 0;
    for (auto &value : pastfps) {
      averagefps += value / pastfps.size();
    }
  }

  bool FramerateManager::govern() {
    float newspf = std::chrono::duration<time_type>(std::chrono::high_resolution_clock::now() - t_past).count();
    //(1 / newspf*ct+averagefps*pastfps.size())/(ct+pastfps.size())
    if (1 / newspf <= idealfps) {
      //ct = 0;
      set();
      return true;
    }
    else {
      return false;
    }
  };

  double FramerateManager::deltatime() const {
    return delta;
  };

  void FramerateManager::manage_fps() {
    if (averagefps > idealfps * .95f && idealfps < maxfps) {
      idealfps += .1f;
    }
    else if (averagefps < idealfps * .93f) {
      idealfps -= .1f;
    }
  };

  float FramerateManager::getfps() const {
    return currentfps;
  };

  float FramerateManager::getavgfps() const {
    return averagefps;
  }

  float FramerateManager::getidealfps() const {
    return idealfps;
  }

  FramerateManager::FramerateManager(float ideal) : idealfps(ideal), maxfps(ideal * 5 / 4) { }
}
