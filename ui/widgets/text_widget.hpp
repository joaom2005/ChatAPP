#ifndef TEXT_BUTTON
#define TEXT_BUTTON

#include "../widget.hpp"
#include "text.hpp"

namespace wWidget {

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

    refreshSize();
  }

  void draw(wGraphics::Renderer &renderer) override {
    for (const auto &child : getChildren()) {
      if (child->isVisible()) {
        child->draw(renderer);
      }
    }

    renderer.drawText(
        getX(), getY(), m_displayText, m_Font, m_TextColor,
        wCommon::TextAlign::Center, getWidth()
    );
  }

  void setText(const std::string &newText) {
    if (m_displayText != newText) {
      m_displayText = newText;

      refreshSize();
    }
  }

private:
  void refreshSize() {
    setWidth(wCommon::measureTextWidth(m_displayText, m_Font));
    setHeight(
        (m_Font.getAscent() - m_Font.getDescent()) *
        wCommon::countLines(m_displayText)
    );
  }

private:
  wCommon::Color m_TextColor;

  const wGraphics::Font &m_Font;
  std::string m_displayText;
};
} // namespace wWidget

#endif // TEXT_BUTTON