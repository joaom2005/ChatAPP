#ifndef CHATAPP_UI_WIDGET_HPP
#define CHATAPP_UI_WIDGET_HPP

#include <memory>
#include <vector>

#include "cursor.hpp"
#include "event.hpp"
#include "renderer.hpp"

namespace wWidget {
class WidgetBase {
public:
  // Virtual functions sub class must implement
  virtual ~WidgetBase() = default;
  virtual void onEvent(const wWindow::Event &event) {
  } // click, hover, key, etc.
  virtual void draw(wGraphics::Renderer &renderer) = 0;
  virtual void layout() {} // recompute children positions, if container

  // Base functions
  void addChild(std::unique_ptr<WidgetBase> child);
  bool hitTest(float x, float y) const;

  // Events
  virtual void onMouseEnter() {}
  virtual void onMouseLeave() {}
  virtual void onMouseDown(wWindow::Key button) {}
  virtual void onMouseUp(wWindow::Key button) {}
  virtual void onClick() {}

  // Getters
  float getX() const;
  float getRelativeX() const;
  float getY() const;
  float getRelativeY() const;
  float getWidth() const;
  float getHeight() const;
  WidgetBase *getParent() const;
  const std::vector<std::unique_ptr<WidgetBase>> &getChildren() const;
  bool isVisible() const;
  bool isFocused() const;
  bool isIteractive() const;

  // Setters
  void setX(float x);
  void setY(float y);
  void setPosition(float x, float y);
  void setWidth(float width);
  void setHeight(float height);
  void setVisible(bool visible);
  void setFocused(bool focused);
  void setIteractive(bool iteractive);

  virtual wCommon::CursorType getCursor() const {
    return wCommon::CursorType::Arrow;
  }

private:
  float x = 0.0f;
  float y = 0.0f;
  float width = 0.0f;
  float height = 0.0f;
  WidgetBase *parent = nullptr;
  std::vector<std::unique_ptr<WidgetBase>> children;
  bool visible = true;
  bool focused = false;
  bool iteractive = false;
};
} // namespace wWidget

#endif // CHATAPP_UI_WIDGET_HPP