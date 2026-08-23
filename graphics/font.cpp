#define STB_TRUETYPE_IMPLEMENTATION
#include "font.hpp"
#include <fstream>
#include <glad/glad.h>
#include <stb_truetype.h>
#include <stdexcept>
#include <vector>

namespace wGraphics {

Font::Font(const std::string &path, float pixelHeight)
    : m_pixelHeight(pixelHeight) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file)
    throw std::runtime_error("Failed to open font: " + path);

  size_t size = file.tellg();
  file.seekg(0);
  std::vector<unsigned char> buffer(size);
  file.read(reinterpret_cast<char *>(buffer.data()), size);

  constexpr int ATLAS_W = 512, ATLAS_H = 512;
  std::vector<unsigned char> atlasPixels(ATLAS_W * ATLAS_H, 0);

  stbtt_bakedchar bakedChars[96]; // ASCII 32..127
  int result = stbtt_BakeFontBitmap(
      buffer.data(), 0, pixelHeight, atlasPixels.data(), ATLAS_W, ATLAS_H, 32,
      96, bakedChars
  );

  stbtt_fontinfo info;
  if (!stbtt_InitFont(
          &info, buffer.data(), stbtt_GetFontOffsetForIndex(buffer.data(), 0)
      ))
    throw std::runtime_error("Failed to init font metrics: " + path);

  float scale = stbtt_ScaleForPixelHeight(&info, pixelHeight);

  int ascentRaw, descentRaw, lineGapRaw;
  stbtt_GetFontVMetrics(&info, &ascentRaw, &descentRaw, &lineGapRaw);

  m_ascent  = ascentRaw * scale;
  m_descent = descentRaw * scale;

  if (result <= 0)
    throw std::runtime_error("Font atlas too small or bake failed");

  for (int i = 0; i < 96; i++) {
    auto &bc = bakedChars[i];
    Glyph g;
    g.u0                     = bc.x0 / (float)ATLAS_W;
    g.v0                     = bc.y0 / (float)ATLAS_H;
    g.u1                     = bc.x1 / (float)ATLAS_W;
    g.v1                     = bc.y1 / (float)ATLAS_H;
    g.width                  = bc.x1 - bc.x0;
    g.height                 = bc.y1 - bc.y0;
    g.bearingX               = bc.xoff;
    g.bearingY               = bc.yoff;
    g.advance                = bc.xadvance;
    m_glyphs[(char)(32 + i)] = g;
  }

  glGenTextures(1, &m_atlasTexture);
  glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RED, ATLAS_W, ATLAS_H, 0, GL_RED, GL_UNSIGNED_BYTE,
      atlasPixels.data()
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Font::~Font() { glDeleteTextures(1, &m_atlasTexture); }

} // namespace wGraphics