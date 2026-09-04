#include "UIManager.hpp"

namespace wWidget {

void UIManager::setRoot(std::unique_ptr<WidgetBase> newRoot) {
  root = std::move(newRoot);
  hovered = nullptr;
  pressed = nullptr;
  captured = nullptr;
  focused = nullptr;
}

bool UIManager::isMouseButton(wWindow::Key key) {
  return key == wWindow::Key::MouseLeftButton ||
         key == wWindow::Key::MouseMiddleButton ||
         key == wWindow::Key::MouseRightButton;
}

WidgetBase *UIManager::hitTest(WidgetBase *node, float x, float y) const {
  if (!node || !node->isVisible()) {
    return nullptr;
  }

  if (!node->hitTest(x, y)) {
    return nullptr;
  }

  const auto &kids = node->getChildren();
  for (auto it = kids.rbegin(); it != kids.rend(); ++it) {
    if (WidgetBase *childHit = hitTest(it->get(), x, y)) {
      return childHit;
    }
  }

  return node->isIteractive() ? node : nullptr;
}

void UIManager::dispatch(const wWindow::Event &event) {
  if (const auto *pos = std::get_if<wWindow::MousePos>(&event)) {
    handleMouseMove(pos->x, pos->y);
  } else if (const auto *key = std::get_if<wWindow::KeyEvent>(&event)) {
    handleKey(key->key, key->pressed);
  }
}

void UIManager::handleMouseMove(int x, int y) {
  lastMouseX = x;
  lastMouseY = y;

  if (captured) {
    captured->onEvent(wWindow::MousePos{x, y});
    return;
  }

  WidgetBase *hit = root ? hitTest(root.get(), (float)x, (float)y) : nullptr;

  if (hit != hovered) {
    if (hovered) {
      hovered->onMouseLeave();
    }
    if (hit) {
      hit->onMouseEnter();
    }
    hovered = hit;
  }
}

void UIManager::handleKey(wWindow::Key key, bool pressed_) {
  if (isMouseButton(key)) {
    if (pressed_) {
      WidgetBase *hit =
          root ? hitTest(root.get(), (float)lastMouseX, (float)lastMouseY)
               : nullptr;

      if (focused && focused != hit) {
        focused->setFocused(false);
      }
      if (hit) {
        hit->setFocused(true);
      }
      focused = hit;

      pressed = hit;
      captured = hit;

      if (hit) {
        hit->onMouseDown(key);
      }
    } else {
      if (!captured) {
        return;
      }

      captured->onMouseUp(key);

      WidgetBase *releasedOver =
          root ? hitTest(root.get(), (float)lastMouseX, (float)lastMouseY)
               : nullptr;
      if (releasedOver == pressed && pressed != nullptr) {
        pressed->onClick();
      }

      captured = nullptr;
      pressed = nullptr;

      // Re-evaluate hover now that the drag/press has ended.
      handleMouseMove(lastMouseX, lastMouseY);
    }
    return;
  }

  // Non-mouse keys (W/A/S/D/Escape): no dedicated virtual on WidgetBase,
  // so route the raw event to whichever widget currently has focus.
  // DESIGN CALL, not confirmed against your intent: unfocused widgets
  // never see keyboard input at all under this scheme.
  if (focused) {
    focused->onEvent(wWindow::KeyEvent{key, pressed_});
  }
}

} // namespace wWidget