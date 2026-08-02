#ifndef __HPP_WIDGET__
#define __HPP_WIDGET__

#include <memory>
#include <vector>

#include "event.hpp"
#include "renderer.hpp"

class Widget {
public:
  virtual ~Widget() = default;
  virtual void onEvent(const wWindow::Event &event) {
  } // click, hover, key, etc.
  virtual void draw(wGraphics::Renderer &renderer) = 0;
  virtual void layout() {} // recompute children positions, if container

  void addChild(std::unique_ptr<Widget> child);
  bool hitTest(float x, float y) const;

  float x, y, width, height;
  Widget *parent = nullptr;
  std::vector<std::unique_ptr<Widget>> children;
  bool visible = true;
  bool focused = false;
};

#endif // __HPP_WIDGET__