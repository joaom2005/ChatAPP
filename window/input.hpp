#ifndef CHATAPP_WINDOW_INPUT_HPP
#define CHATAPP_WINDOW_INPUT_HPP

#include <array>
#include <memory>

#include "event.hpp"

namespace wWindow {
class Input {
  static constexpr size_t KEY_COUNT = static_cast<size_t>(Key::COUNT);
  static constexpr size_t idx(Key k) { return static_cast<size_t>(k); }

  std::shared_ptr<EventQueue> m_queue;

  std::array<bool, KEY_COUNT> m_keys{};
  std::array<bool, KEY_COUNT> m_keysPrev{};

  int m_mouseDx = 0, m_mouseDy = 0;
  int m_mouseX = 0, m_mouseY = 0;

public:
  explicit Input(std::shared_ptr<EventQueue> queue)
      : m_queue(std::move(queue)) {}

  void beginFrame() {
    m_keysPrev = m_keys;
    m_mouseDx = m_mouseDy = 0;
  }

  void feed(const Event &e) {
    std::visit([this](auto &&ev) { handle(ev); }, e);
  }

  bool isKeyDown(Key k) const { return m_keys[idx(k)]; }
  bool isKeyPressed(Key k) const {
    return m_keys[idx(k)] && !m_keysPrev[idx(k)];
  }
  bool isKeyReleased(Key k) const {
    return !m_keys[idx(k)] && m_keysPrev[idx(k)];
  }

private:
  void handle(const KeyEvent &e) { m_keys[idx(e.key)] = e.pressed; }
  void handle(const MousePos &e) {
    m_mouseDx += e.x - m_mouseX;
    m_mouseX = e.x;

    m_mouseDy += e.y - m_mouseY;
    m_mouseY = e.y;
  }
};
} // namespace wWindow

#endif // CHATAPP_WINDOW_INPUT_HPP