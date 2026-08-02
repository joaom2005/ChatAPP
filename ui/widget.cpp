#include "widget.hpp"

void Widget::addChild(std::unique_ptr<Widget> child) {
  if (child == nullptr) {
    return;
  }

  child->parent = this;
  children.push_back(std::move(child));
}

bool Widget::hitTest(float x, float y) const {
  return x >= this->x && x <= this->x + this->width && y >= this->y &&
         y <= this->y + this->height;
}
