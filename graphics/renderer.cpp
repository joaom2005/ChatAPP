#include "renderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace wGraphics {

Renderer::Renderer() {
  // unit quad, two triangles
  float verts[] = {
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  };

  glGenVertexArrays(1, &m_rectVAO);
  glGenBuffers(1, &m_rectVBO);

  glBindVertexArray(m_rectVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_rectVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  // load .vert/.frag strings
  m_rectShader = std::make_unique<Shader>(rectVertSrc, rectFragSrc);
}

void Renderer::beginFrame(int width, int height, Color clearColor) {
  m_width = width;
  m_height = height;
  glViewport(0, 0, width, height);
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND); // needed for text alpha blending later too
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::drawRect(float x, float y, float w, float h, Color color) {
  glm::mat4 proj = glm::ortho(0.0f, (float)m_width, (float)m_height, 0.0f);

  m_rectShader->use();
  m_rectShader->setMat4("uProjection", proj);
  m_rectShader->setVec4("uRect", x, y, w, h);
  m_rectShader->setVec4("uColor", color.r, color.g, color.b, color.a);

  glBindVertexArray(m_rectVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::drawText(float x, float y, const std::string &text, Font &font,
                        Color color) {
  std::vector<float> verts; // x, y, u, v per vertex, 6 verts per glyph

  float cursorX = x;
  float cursorY = y + font.getPixelHeight(); // baseline

  for (char c : text) {
    const Glyph &g = font.getGlyph(c);

    float x0 = cursorX + g.bearingX;
    float y0 = cursorY + g.bearingY;
    float x1 = x0 + g.width;
    float y1 = y0 + g.height;

    verts.insert(verts.end(),
                 {
                     x0, y0, g.u0, g.v0, x1, y0, g.u1, g.v0, x1, y1, g.u1, g.v1,
                     x0, y0, g.u0, g.v0, x1, y1, g.u1, g.v1, x0, y1, g.u0, g.v1,
                 });

    cursorX += g.advance;
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(),
               GL_DYNAMIC_DRAW);

  glm::mat4 proj = glm::ortho(0.0f, (float)m_width, (float)m_height, 0.0f);
  m_textShader->use();
  m_textShader->setMat4("uProjection", proj);
  m_textShader->setVec4("uColor", color.r, color.g, color.b, color.a);
  m_textShader->setInt("uAtlas", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font.getAtlasTexture());

  glBindVertexArray(m_textVAO);
  glDrawArrays(GL_TRIANGLES, 0, verts.size() / 4);
}

void Renderer::endFrame() {}

} // namespace wGraphics