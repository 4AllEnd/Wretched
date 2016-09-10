#pragma once
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
namespace Wretched {
  //custom
  struct Thread : public std::thread {
    template <typename _Fn>
    struct Functor {
      Functor(_Fn _function) : function(_function) {}
      _Fn function;
      template <typename ..._Args>
      void operator()(Thread *thread, _Args &&...args) {
        Thread &_thread = *thread;
        _thread.executing = true;
        function(std::forward<_Args>(args)...);
        _thread.executing = false;
      }
    };
    Thread() : std::thread([]{}) {}

    ~Thread() {
      detach();
    }

    void detach() {
      if (joinable()) {
        std::thread::detach();
      }
    }

    template <typename _Fn, typename ... _Args>
    Thread(_Fn &&function, _Args &&...args)
      : std::thread(Functor<_Fn>(function), this, std::forward<_Args>(args)...) {}

    template <typename _Fn, typename ... _Args>
    void operator()(_Fn &&function, _Args &&...args) {
      std::thread a(Functor<_Fn>(function), this, std::forward<_Args>(args)...);
      swap(a);
      a.detach();
    }

    bool busy() const {
      return executing.load();
    }

  protected:
    std::atomic<bool> executing;
  };

  struct ThreadPool {
    std::vector<std::unique_ptr<Thread>> threads;
    ThreadPool(unsigned int size = 8) : threads(size){}
    template <typename _Fn, typename ... _Args>
    bool consume(_Fn &&function, _Args &&...args) {
      for (auto &each : threads) {
        if (!each->busy()) {
          each.get()->operator()(function, std::forward<_Args>(args)...);
          return true;
        }
      }
      threads.push_back(std::move(std::unique_ptr<Thread>(new Thread(function, std::forward<_Args>(args)...))));
      return false;
    }
  };
};

