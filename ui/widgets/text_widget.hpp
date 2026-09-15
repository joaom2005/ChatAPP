#ifndef TEXT_BUTTON
#define TEXT_BUTTON

#include "../widget.hpp"

namespace wWidget {

static float
measureTextWidth(const std::string &text, const wGraphics::Font &font) {
  float maxWidth  = 0.0f;
  float lineWidth = 0.0f;

  for (char c : text) {
    if (c == '\r') {
      continue;
    }
    if (c == '\n') {
      maxWidth  = std::max(maxWidth, lineWidth);
      lineWidth = 0.0f;
      continue;
    }
    lineWidth += font.getGlyph(c).advance;
  }

  return std::max(maxWidth, lineWidth); // catch the last line (no trailing \n)
}

class TextWidget : public WidgetBase {
public:
  TextWidget(
      float x, float y, const std::string text, const wGraphics::Font &font,
      const wCommon::Color textColor
  )
      : WidgetBase{}, m_TextColor(textColor), m_Font(font),
        m_displayText(std::move(text)) {
    setX(x);
    setY(y);

    setWidth(measureTextWidth(m_displayText, font));
    setHeight(font.getAscent() - font.getDescent());
  }

  void draw(wGraphics::Renderer &renderer) override {
    for (const auto &child : getChildren()) {
      if (child->isVisible()) {
        child->draw(renderer);
      }
    }

    renderer.drawText(getX(), getY(), m_displayText, m_Font, m_TextColor);
  }

  void setText(const std::string &newText) {
    if (m_displayText != newText) {
      m_displayText = newText;
      setWidth(measureTextWidth(m_displayText, m_Font));
    }
  }

private:
  wCommon::Color m_TextColor;

  const wGraphics::Font &m_Font;
  std::string m_displayText;
};
} // namespace wWidget

#endif // TEXT_BUTTON