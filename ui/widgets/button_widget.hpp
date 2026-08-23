#ifndef __WIDGET_BUTTON__
#define __WIDGET_BUTTON__

#include "../widget.hpp"

#include "text_widget.hpp"

namespace wWidget {
class ButtonWidget : public WidgetBase {
public:
  ButtonWidget(
      const float x, const float y, const float width, const float height,
      const wGraphics::Color BackgroundColor, const std::string &text,
      const wGraphics::Font &font, const wGraphics::Color textColor
  )
      : WidgetBase{}, m_BackgroundColor(BackgroundColor) {
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);

    setIteractive(true);

    auto textWidget =
        std::make_unique<TextWidget>(0.0f, 0.0f, text, font, textColor);
    textWidget->setX((width - textWidget->getWidth()) / 2.0f);
    textWidget->setY((height - textWidget->getHeight()) / 2.0f);
    m_label = textWidget.get();
    addChild(std::move(textWidget));
  }

  void draw(wGraphics::Renderer &renderer) override {
    // First draw the rect
    renderer.drawRect(
        getX(), getY(), getWidth(), getHeight(), m_BackgroundColor
    );

    // Then draw the children
    for (const auto &child : getChildren()) {
      if (child->isVisible()) {
        child->draw(renderer);
      }
    }
  }

private:
  wGraphics::Color m_BackgroundColor;

  TextWidget *m_label = nullptr;
};
} // namespace wWidget

#endif // __WIDGET_BUTTON__