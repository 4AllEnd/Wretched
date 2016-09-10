#include <Wretched/Math/GridPosition.h>
#include <iostream>

namespace Wretched {
  GridPosition::GridPosition() {}

  void GridPosition::move(gridvec a_) {
    localposition += a_;
    if ((localposition.x < -limit) || (localposition.x > limit)) {
      int r_;
      localposition.x = remquof(localposition.x, limit, &r_);
      sector.x += r_;
    }
    if ((localposition.y < -limit) || (localposition.y > limit)) {
      int r_;
      localposition.y = remquof(localposition.y, limit, &r_);
      sector.y += r_;
    }
    if ((localposition.z < -limit) || (localposition.z > limit)) {
      int r_;
      localposition.z = remquof(localposition.z, limit, &r_);
      sector.z += r_;
    }
  }

  float& GridPosition::getlimit() {
    return limit;
  }

  gridvec& GridPosition::getsector() {
    return sector;
  }

  gridvec GridPosition::getabsposition() {
    return sector * limit + localposition;
  }

  gridvec& GridPosition::getlocalposition() {
    return localposition;
  }

  void GridPosition::operator=(gridvec a_) {
    localposition = a_;
  }

  GridPosition GridPosition::operator+(gridvec a_) {
    GridPosition b = *this;
    b.move(a_);
    return b;
  }

  GridPosition GridPosition::operator-(gridvec a_) {
    GridPosition b = *this;
    b.move(-a_);
    return b;
  }

  void GridPosition::operator+=(gridvec a_) {
    move(a_);
  }

  void GridPosition::operator-=(gridvec a_) {
    move(-a_);
  }

  template <typename T>
  GridPosition& GridPosition::operator*=(T a_) {
    return this * a_;
  }

  bool GridPosition::operator==(GridPosition a_) {
    return (a_.getsector() == sector && a_.getlocalposition() == localposition) ? true : false;
  }

  bool GridPosition::operator!=(GridPosition a_) {
    return !(a_ == *this);
  }

  GridPosition GridPosition::operator-() {
    return GridPosition(-sector, -localposition);
  }

  GridPosition GridPosition::operator-(GridPosition a_) {
    GridPosition nv(sector - a_.sector, localposition);
    nv.move(-a_.localposition);
    return nv;
  }

  GridPosition GridPosition::operator+(GridPosition a_) {
    a_.sector += sector;
    a_.move(localposition);
    return a_;
  }

  void GridPosition::operator=(GridPosition a_) {
    sector = a_.sector;
    localposition = a_.localposition;
  }
  GridPosition::GridPosition(gridivec _sector, gridvec position): sector(_sector), localposition(position) {};

  std::ostream& operator<<(std::ostream &a_, GridPosition &b_) {
    gridivec c_ = b_.getsector();
    gridvec d_ = b_.getlocalposition();
    return a_ << "sector: [" << c_.x << "," << c_.y
        << "," << c_.z << "]" << "lp: (" << d_.x << "," <<
        d_.y << "," << d_.z << ")";
  }


}

