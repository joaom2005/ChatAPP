#ifndef __HPP_INPUT__
#define __HPP_INPUT__

#include <memory>

#include "event.hpp"
#include "window.hpp"

class Input {
  std::shared_ptr<EventQueue> m_queue;
  std::unordered_map<Key, bool> m_keys;
  std::unordered_map<Key, bool> m_keysPrev;
  int m_mouseDx = 0, m_mouseDy = 0;

public:
  explicit Input(std::shared_ptr<EventQueue> queue)
      : m_queue(std::move(queue)) {}

  void update() {
    m_keysPrev = m_keys;
    m_mouseDx = m_mouseDy = 0;

    Event e;
    while (m_queue->poll(e)) {
      std::visit([this](auto &&ev) { handle(ev); }, e);
    }
  }

  bool isKeyDown(Key k) const { return m_keys.count(k) && m_keys.at(k); }
  bool isKeyPressed(Key k) const {
    return isKeyDown(k) && !wasPrev(k);
  } // this frame only
  bool isKeyReleased(Key k) const { return !isKeyDown(k) && wasPrev(k); }

private:
  void handle(const KeyEvent &e) { m_keys[e.key] = e.pressed; }
  void handle(const MouseMove &e) {
    m_mouseDx += e.dx;
    m_mouseDy += e.dy;
  }
  void handle(const MouseButton &) { /* ... */ }

  bool wasPrev(Key k) const { return m_keysPrev.count(k) && m_keysPrev.at(k); }
};

#endif // __HPP_INPUT__