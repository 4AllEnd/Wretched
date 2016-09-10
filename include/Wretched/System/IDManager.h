namespace Wretched {
  template<typename T = long long>
  struct IDManager {
    T assign() {
      if (removed.size() != 0) {
        T last = removed.back();
        removed.pop_back();
        return last;
      }
      return ++nextid;
    }

    void remove(T &_id) {
      removed.push_back(_id);
    }

  protected:
    T nextid = 0;
    std::vector<T> removed;
  };
}
