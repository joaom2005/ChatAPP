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

bool WidgetBase::hitTest(float px, float py) const {
  float absX = getX();
  float absY = getY();
  return px >= absX && px <= absX + width && py >= absY && py <= absY + height;
}

// Getters
float WidgetBase::getX() const { return parent ? parent->getX() + x : x; }
float WidgetBase::getRelativeX() const { return x; }
float WidgetBase::getY() const { return parent ? parent->getY() + y : y; }
float WidgetBase::getRelativeY() const { return y; }
float WidgetBase::getWidth() const { return width; }
float WidgetBase::getHeight() const { return height; }

WidgetBase *WidgetBase::getParent() const { return parent; }
const std::vector<std::unique_ptr<WidgetBase>> &
WidgetBase::getChildren() const {
  return children;
}

bool WidgetBase::isVisible() const { return visible; }
bool WidgetBase::isFocused() const { return focused; }
bool WidgetBase::isIteractive() const { return iteractive; }

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
void WidgetBase::setIteractive(bool value) { iteractive = value; }

}; // namespace wWidget