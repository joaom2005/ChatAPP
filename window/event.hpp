#ifndef __HPP_EVENT__
#define __HPP_EVENT__

#include <queue>
#include <variant>

namespace wWindow {
enum class Key {
  MouseLeftButton = 0,
  MouseMiddleButton,
  MouseRightButton,

  W,
  A,
  S,
  D,
  Escape,
  COUNT
};

struct KeyEvent {
  Key key;
  bool pressed;
};
struct MousePos {
  int x, y;
};

using Event = std::variant<KeyEvent, MousePos>;

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
} // namespace wWindow

#endif // __HPP_EVENT__