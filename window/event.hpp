#ifndef __HPP_EVENT__
#define __HPP_EVENT__

#include <queue>
#include <variant>

enum class Key {
  W,
  A,
  S,
  D,
  Escape,
};

struct KeyEvent {
  Key key;
  bool pressed;
};
struct MouseMove {
  int dx, dy;
};
struct MouseButton {
  int button;
  bool pressed;
};

using Event = std::variant<KeyEvent, MouseMove, MouseButton>;

class EventQueue {
  std::queue<Event> m_events;

public:
  void push(const Event &e) { m_events.push(e); }

  bool poll(Event &out) {
    if (m_events.empty())
      return false;
    out = m_events.front();
    m_events.pop();
    return true;
  }
};

#endif // __HPP_EVENT__