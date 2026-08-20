#pragma once
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

  const Glyph &getGlyph(char c) const { return m_glyphs.at(c); }
  unsigned int getAtlasTexture() const { return m_atlasTexture; }
  float getPixelHeight() const { return m_pixelHeight; }

private:
  unsigned int m_atlasTexture = 0;
  float m_pixelHeight;
  std::unordered_map<char, Glyph> m_glyphs;
};

} // namespace wGraphics