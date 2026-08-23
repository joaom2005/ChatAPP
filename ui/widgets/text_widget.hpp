#ifndef __TEXT_BUTTON__
#define __TEXT_BUTTON__

#include "../widget.hpp"

namespace wWidget {

// This is an estimate; exact width depends on each glyph's font metrics.
static float getTextSize(float fontSize, float length) {
  return length * fontSize * 0.5f;
}

class TextWidget : public WidgetBase {
public:
  TextWidget(
      float x, float y, const std::string &text, const wGraphics::Font &font,
      const wGraphics::Color textColor
  )
      : WidgetBase{}, m_TextColor(textColor), m_Font(font),
        m_displayText(std::move(text)) {
    setX(x);
    setY(y);

    setWidth(getTextSize(font.getPixelHeight(), text.length()));
    setHeight(font.getPixelHeight());
  }

  void draw(wGraphics::Renderer &renderer) override {
    for (const auto &child : getChildren()) {
      if (child->isVisible()) {
        child->draw(renderer);
      }
    }

    renderer.drawText(getX(), getY(), m_displayText, m_Font, m_TextColor);
  }

private:
  wGraphics::Color m_TextColor;

  const wGraphics::Font &m_Font;
  std::string m_displayText;
};
} // namespace wWidget

#endif // __TEXT_BUTTON__