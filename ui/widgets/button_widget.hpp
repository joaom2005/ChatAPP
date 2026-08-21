#ifndef __WIDGET_BUTTON__
#define __WIDGET_BUTTON__

#include "../widget.hpp"

namespace wWidget {
class ButtonWidget : public WidgetBase {
public:
  ButtonWidget(
      const float x, const float y, const float width, const float height,
      const wGraphics::Color BackgroundColor, const std::string text,
      const wGraphics::Font &font, const wGraphics::Color textColor
  )
      : WidgetBase{}, m_BackgroundColor(BackgroundColor),
        m_TextColor(textColor), m_Font(font), m_displayText(std::move(text)) {
    this->x      = x;
    this->y      = y;
    this->width  = width;
    this->height = height;
  }

  void draw(wGraphics::Renderer &renderer) override {
    for (const auto &child : getChildren()) {
      if (child->isVisible()) {
        child->draw(renderer);
      }
    }

    renderer.drawRect(x, y, width, height, m_BackgroundColor);
    renderer.drawText(x, y, m_displayText, m_Font, m_TextColor);
  }

protected:
  wGraphics::Color m_BackgroundColor;
  wGraphics::Color m_TextColor;

  const wGraphics::Font &m_Font;
  std::string m_displayText;
};
} // namespace wWidget

#endif // __WIDGET_CONTAINER__