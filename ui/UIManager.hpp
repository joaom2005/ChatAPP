#ifndef CHATAPP_UI_UIMANAGER_HPP
#define CHATAPP_UI_UIMANAGER_HPP

#include "event.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "widgets/container_widget.hpp"
#include "window.hpp"

#include <memory>

namespace wWidget {
class UIManager {
public:
  void setRoot(std::unique_ptr<WidgetBase> root);

  // main's event loop calls this once per polled wWindow::Event.
  void dispatch(const wWindow::Event &event);

private:
  void handleMouseMove(int x, int y);
  void handleKey(wWindow::Key key, bool pressed);

  static bool isMouseButton(wWindow::Key key);

  WidgetBase *hitTest(WidgetBase *node, float x, float y) const;

  std::unique_ptr<WidgetBase> root;

  WidgetBase *hovered = nullptr; // for enter/leave
  WidgetBase *pressed =
      nullptr; // widget that got mouseDown, for click detection
  WidgetBase *captured = nullptr; // widget owning input during a drag
  WidgetBase *focused = nullptr;  // widget that gets non-mouse Key events

  int lastMouseX = 0;
  int lastMouseY = 0;
};
} // namespace wWidget

#endif // CHATAPP_UI_UIMANAGER_HPP