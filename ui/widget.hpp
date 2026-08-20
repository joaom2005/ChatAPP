#ifndef __HPP_WIDGET__
#define __HPP_WIDGET__

#include <memory>
#include <vector>

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

  // Getters
  float getX() const;
  float getY() const;
  float getWidth() const;
  float getHeight() const;
  WidgetBase *getParent() const;
  const std::vector<std::unique_ptr<WidgetBase>> &getChildren() const;
  bool isVisible() const;
  bool isFocused() const;

  // Setters
  void setX(float x);
  void setY(float y);
  void setPosition(float x, float y);
  void setWidth(float width);
  void setHeight(float height);
  void setVisible(bool visible);
  void setFocused(bool focused);

private:
  float x            = 0.0f;
  float y            = 0.0f;
  float width        = 0.0f;
  float height       = 0.0f;
  WidgetBase *parent = nullptr;
  std::vector<std::unique_ptr<WidgetBase>> children;
  bool visible = true;
  bool focused = false;
};
} // namespace wWidget

#endif // __HPP_WIDGET__