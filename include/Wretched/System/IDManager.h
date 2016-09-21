namespace Wretched {
  struct SID;
  // Serial number generator
  template<typename T = long long>
  struct IDManager {
    SID createSID() {
      return SID(*this);
    }
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
  // Serial ID
  template<typename T = long long>
  struct SID {
    // destructor removes the id from the managing class
    ~SID(){
      _generator.remove(id);
    }
    friend struct IDManager;
    private:
      T id;
      IDManager generator;
      // assigns the class that generated it to an internal variable for deletion.
      // also assigns id.
      SID(IDManager &_generator) : generator(_generator) {
        id = _generator.assign();
      }
      SID(const SID& _a) : _generator(_a.generator), id(_a.id) {}
  };
}
