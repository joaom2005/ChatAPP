#include "renderer.hpp"
#include <glad/glad.h>

namespace wGraphics {

void Renderer::beginFrame(int width, int height, Color clearColor) {
  glViewport(0, 0, width, height);
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame() {}

void Renderer::drawRect(float x, float y, float w, float h, Color color) {
  (void)x;
  (void)y;
  (void)w;
  (void)h;
  (void)color;
}

void Renderer::drawText(float x, float y, const std::string &text, Font &font,
                        Color color) {
  (void)x;
  (void)y;
  (void)text;
  (void)font;
  (void)color;
}

} // namespace wGraphics