#ifndef CHATAPP_COMMON_TEXT_HPP
#define CHATAPP_COMMON_TEXT_HPP

#include <string>
#include <font.hpp>

namespace wCommon {

enum class TextAlign { Left, Right, Center, Fill };

inline float
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

  return std::max(maxWidth, lineWidth);
}

inline int countLines(const std::string &text) {
  int lines = 1;
  for (char c : text) {
    if (c == '\n') {
      lines++;
    }
  }
  return lines;
}

} // namespace wCommon

#endif // CHATAPP_COMMON_TEXT_HPP