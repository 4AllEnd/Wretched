#pragma once
#include <vector>
template <typename T>
struct Node {
  T* parent = nullptr;
  T* root = nullptr;
  std::vector<T*> children;
  unsigned int numdescendants(T &element) const;
  void setparent(T &_target);
  bool isroot() const;
};

template <typename T>
void Node<T>::setparent(T &_target) {
  parent = &_target;
  root = &_target.root;
}
template <typename T>
bool Node<T>::isroot() const {
  return parent == nullptr;
}
template <typename T>
unsigned int Node<T>::numdescendants(T &element) const{
  unsigned int num = 0;
  for (auto &each : element.children)num += 1 + numdescendants(*each);
  return num;
}