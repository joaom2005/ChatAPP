#ifndef CHATAPP_GRAPHICS_FONT_HPP
#define CHATAPP_GRAPHICS_FONT_HPP
#include <string>
#include <unordered_map>

namespace wGraphics {

struct Glyph {
  float u0, v0, u1, v1;     // texture coords in the atlas
  float width, height;      // glyph size in pixels
  float bearingX, bearingY; // offset from cursor to glyph top-left
  float advance;            // how far to move cursor after this glyph
};

class Font {
public:
  Font(const std::string &path, float pixelHeight);
  ~Font();

  Font(const Font &)            = delete;
  Font &operator=(const Font &) = delete;
  Font(Font &&)                 = delete;
  Font &operator=(Font &&)      = delete;

  const Glyph &getGlyph(char c) const {
    auto it = m_glyphs.find(c);
    if (it == m_glyphs.end()) {
      return m_glyphs.at('?'); // fallback glyph — always exists, ASCII 63
    }
    return it->second;
  }
  unsigned int getAtlasTexture() const { return m_atlasTexture; }
  float getPixelHeight() const { return m_pixelHeight; }

  float getAscent() const { return m_ascent; }
  float getDescent() const { return m_descent; }

private:
  unsigned int m_atlasTexture = 0;
  float m_pixelHeight;
  std::unordered_map<char, Glyph> m_glyphs;

  float m_ascent  = 0.0f;
  float m_descent = 0.0f;
};

} // namespace wGraphics

#endif // CHATAPP_GRAPHICS_FONT_HPP