#ifndef __WIDGET_CONTAINER__
#define __WIDGET_CONTAINER__

#include "../widget.hpp"

namespace wWidget {
class ContainerWidget : public WidgetBase {
public:
  void draw(wGraphics::Renderer &renderer) override {
    for (const auto &child : getChildren()) {
      if (child->isVisible()) {
        child->draw(renderer);
      }
    }
  }
};
} // namespace wWidget

#endif // __WIDGET_CONTAINER__