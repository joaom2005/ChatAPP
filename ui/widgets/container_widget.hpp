#ifndef WIDGET_CONTAINER
#define WIDGET_CONTAINER

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

#endif // WIDGET_CONTAINER