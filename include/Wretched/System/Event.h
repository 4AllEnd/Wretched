#pragma once
#include <vector>
#include <memory>
#include <algorithm>

namespace Wretched {
  struct Subscriber;

  struct Event {
    friend Subscriber;
    Event();
    ~Event();
    virtual void notify();
  protected:
    std::vector<Subscriber*> subscribers;
  };

  struct Subscriber {
    friend Event;
    Subscriber();//default constructor
    Subscriber(Event &event);
    virtual ~Subscriber();
    virtual void acknowledge(){};//acknowledge notification with responsive behavior
    void subscribe(Event &event);
    void unsubscribe(Event &event);
  protected:
    std::vector<Event*> events;
  };
  template <typename T>
  struct Messenger : Event, Subscriber {
    T* ptr;
    Messenger(T& object) : ptr(object) {};
  };
}
