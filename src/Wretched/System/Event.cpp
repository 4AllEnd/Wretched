#include <Wretched/System/Event.h>

namespace Wretched {
  Event::~Event() {
    for (auto &each : subscribers)each->events.erase(std::find(each->events.begin(), each->events.end(), this));
  }

  void Event::notify() {
    for (auto &each : subscribers)each->acknowledge();
  }

  Subscriber::Subscriber() { }

  Subscriber::Subscriber(Event &event) {
    subscribe(event);
  }

  Subscriber::~Subscriber() {
    for (auto &each : events)each->subscribers.erase(std::find(each->subscribers.begin(), each->subscribers.end(), this));
  }

  void Subscriber::subscribe(Event &event) {
    if (std::find(events.begin(), events.end(), (Wretched::Event*)&event) == events.end())events.push_back((Wretched::Event*)&event);
    if (std::find(event.subscribers.begin(), event.subscribers.end(), (Wretched::Subscriber*)this) == event.subscribers.end())event.subscribers.push_back((Wretched::Subscriber*)this);
  }

  void Subscriber::unsubscribe(Event &event) {
    event.subscribers.erase(std::find(event.subscribers.begin(), event.subscribers.end(), this));
    events.erase(std::find(events.begin(), events.end(), &event));
  }
}

