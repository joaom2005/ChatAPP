#ifndef WIDGET_BUTTON
#define WIDGET_BUTTON

#include "../widget.hpp"

#include "color.hpp"
#include "text_widget.hpp"
#include <X11/X.h>
#include <iostream>

namespace wWidget {
class ButtonWidget : public WidgetBase {
public:
  ButtonWidget(const float x, const float y, const float width,
               const float height, const std::string &text,
               const wGraphics::Font &font)
      : WidgetBase{} {
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    setIteractive(true);

    auto textWidget = std::make_unique<TextWidget>(
        0.0f, 0.0f, text, font, wCommon::Color{0.0f, 0.0f, 0.0f, 1.0f});
    textWidget->setX((width - textWidget->getWidth()) / 2.0f);
    textWidget->setY((height - textWidget->getHeight()) / 2.0f);
    m_label = textWidget.get();
    addChild(std::move(textWidget));
  }

  void draw(wGraphics::Renderer &renderer) override {
    // First draw the rect
    if (isFocused()) {
      renderer.drawRect(getX(), getY(), getWidth(), getHeight(),
                        m_BackgroundColor);
    } else {
      renderer.drawRect(getX(), getY(), getWidth(), getHeight(),
                        m_HighlightedColor);
    }

    // Then draw the children
    for (const auto &child : getChildren()) {
      if (child->isVisible()) {
        child->draw(renderer);
      }
    }
  }

  void onMouseEnter() override { setFocused(true); }
  void onMouseLeave() override { setFocused(false); }

  wCommon::CursorType getCursor() const override {
    return wCommon::CursorType::Hand;
  }

private:
  wCommon::Color m_BackgroundColor = wCommon::Color{0.94f, 0.94f, 0.94f, 1.0f};
  wCommon::Color m_HighlightedColor = wCommon::Color{0.87f, 0.87f, 0.87f, 1.0f};

  TextWidget *m_label = nullptr;
};
} // namespace wWidget

#endif // WIDGET_BUTTON