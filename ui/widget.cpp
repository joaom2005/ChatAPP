#include "widget.hpp"

namespace wWidget {
// Base functions
void WidgetBase::addChild(std::unique_ptr<WidgetBase> child) {
  if (child == nullptr) {
    return;
  }

  child->parent = this;
  children.push_back(std::move(child));
}

bool WidgetBase::hitTest(float x, float y) const {
  return x >= this->x && x <= this->x + this->width && y >= this->y &&
         y <= this->y + this->height;
}

// Getters
float WidgetBase::getX() const { return x; }
float WidgetBase::getY() const { return y; }
float WidgetBase::getWidth() const { return width; }
float WidgetBase::getHeight() const { return height; }

WidgetBase *WidgetBase::getParent() const { return parent; }
const std::vector<std::unique_ptr<WidgetBase>> &
WidgetBase::getChildren() const {
  return children;
}

bool WidgetBase::isVisible() const { return visible; }
bool WidgetBase::isFocused() const { return focused; }

// Setters
void WidgetBase::setX(float value) { x = value; }
void WidgetBase::setY(float value) { y = value; }
void WidgetBase::setPosition(float x, float y) {
  this->x = x;
  this->y = y;
}

void WidgetBase::setWidth(float value) { width = value; }
void WidgetBase::setHeight(float value) { height = value; }
void WidgetBase::setVisible(bool value) { visible = value; }
void WidgetBase::setFocused(bool value) { focused = value; }

}; // namespace wWidget