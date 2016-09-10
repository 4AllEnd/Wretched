#pragma once

namespace Wretched {
  //target-value that will be modified. 
  //source-value that will replace the digits where allowed by the mask.
  //mask-value allows the target to be modified; a digit set to one allows the target to be edited in the corresponding place.
  template <typename T, typename A, typename G>
  void bitassign(T &target, const A &source, const G &mask) {
    target ^= (source ^ target) & mask;
  }

  //return digits where target and mask are both 
  template <typename T, typename A>
  T bitall(const T &target, const A &mask) {
    return (target & mask) == mask;
  }

  template <typename T, typename A>
  bool bitany(T &target, A &mask) {
    return (target & mask) != 0;
  }
}

