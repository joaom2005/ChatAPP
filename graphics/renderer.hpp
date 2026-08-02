#ifndef __HPP_GRAPHICS__
#define __HPP_GRAPHICS__

#include <string>

namespace wGraphics {

struct Color {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 1.0f;
};

struct Font {};

class Renderer {
public:
  void beginFrame(int width, int height, Color clearColor);
  void drawRect(float x, float y, float w, float h, Color color);
  void drawText(float x, float y, const std::string &text, Font &font,
                Color color);
  void endFrame(); // flush batched quads to GL
private:
  // batched vertex buffer, one draw call per texture/shader combo
};

} // namespace wGraphics

#endif // __HPP_GRAPHICS__