#pragma once
#include <math.h>
#define GLM_FORCE_RADIANS
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <ostream>
#define W_SECTORLIMIT 15000.f

//#define W_SECTORLIMIT 1000000.f
namespace Wretched {
  typedef glm::vec3 gridvec;
  typedef glm::vec3 gridivec;

  struct GridPosition {
    GridPosition();
    GridPosition(gridivec _sector, gridvec _position);
    void move(gridvec a_);
    float& getlimit();
    gridvec& getsector();
    gridvec getabsposition();
    gridvec& getlocalposition();
    void operator=(gridvec a_);
    void operator=(GridPosition a_);
    GridPosition operator-();
    GridPosition operator+(gridvec a_);
    GridPosition operator-(gridvec a_);
    void operator+=(gridvec a_);
    void operator-=(gridvec a_);
    bool operator==(GridPosition a_);
    bool operator!=(GridPosition a_);
    GridPosition operator-(GridPosition a_);
    GridPosition operator+(GridPosition a_);
    template <typename T>
    GridPosition& operator*=(T a_);
  protected:
#ifdef W_SECTORLIMIT
    float limit = W_SECTORLIMIT;//16.25 million is a good limit for float because it leaves for a .5 million change in velocity
#else
		float limit = 150000.f;
#endif
    gridivec sector = gridivec(0, 0, 0);
    gridvec localposition = gridvec(0, 0, 0);
  };

  template <typename G>
  typename std::enable_if<std::is_arithmetic<G>::value, GridPosition>::type operator*(const GridPosition &b_, const G a_) {
    GridPosition b = b_;
    gridivec q;
    gridvec r(remquof(b.getsector().x * a_, 1.f, &q.x), remquof(b.getsector().y * a_, 1.f, &q.y), remquof(b.getsector().z * a_, 1.f, &q.z));
    b.getsector() = q;
    b.getlocalposition() *= a_;
    b.move(r * b.getlimit());
    return b;
  }

  std::ostream& operator<<(std::ostream &a_, GridPosition &b_);
}

