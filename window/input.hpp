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
  int m_mouseX = 0, m_mouseY = 0;

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

  bool isKeyDown(Key k) const {
    auto it = m_keys.find(k);
    return it != m_keys.end() && it->second;
  }
  bool isKeyPressed(Key k) const {
    return isKeyDown(k) && !wasPrev(k);
  } // this frame only
  bool isKeyReleased(Key k) const { return !isKeyDown(k) && wasPrev(k); }

private:
  void handle(const KeyEvent &e) { m_keys[e.key] = e.pressed; }
  void handle(const MouseMove &e) {
    m_mouseDx += e.dx;
    m_mouseDy += e.dy;
    m_mouseX += e.dx;
    m_mouseY += e.dy;
  }

  bool wasPrev(Key k) const {
    auto it = m_keysPrev.find(k);
    return it != m_keysPrev.end() && it->second;
  }
};

#endif // __HPP_INPUT__