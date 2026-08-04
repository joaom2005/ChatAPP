#ifndef __HPP_GRAPHICS__
#define __HPP_GRAPHICS__

#include "font.hpp"
#include "shader.hpp"
#include <memory>

namespace wGraphics {

struct Color {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 1.0f;
};

class Renderer {
public:
  Renderer();
  void beginFrame(int width, int height, Color clearColor);
  void endFrame();

  void drawRect(float x, float y, float w, float h, Color color);
  void drawText(float x, float y, const std::string &text, Font &font,
                Color color);

private:
  int m_width = 0, m_height = 0;

  std::unique_ptr<Shader> m_rectShader;
  unsigned int m_rectVAO = 0, m_rectVBO = 0;

  std::unique_ptr<Shader> m_textShader;
  unsigned int m_textVAO = 0, m_textVBO = 0;

  inline static constexpr const char *rectVertSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;

uniform mat4 uProjection;
uniform vec4 uRect;

void main() {
  vec2 pos = uRect.xy + aPos * uRect.zw;
  gl_Position = uProjection * vec4(pos, 0.0, 1.0);
}
)";

  inline static constexpr const char *rectFragSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;

void main() {
  FragColor = uColor;
}
)";

  inline static constexpr const char *textVertSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 vUV;
uniform mat4 uProjection;

void main() {
  vUV = aUV;
  gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}
)";

  inline static constexpr const char *textFragSrc = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uAtlas;
uniform vec4 uColor;

void main() {
  float alpha = texture(uAtlas, vUV).r;
  FragColor = vec4(uColor.rgb, uColor.a * alpha);
}
)";
};

} // namespace wGraphics

#endif // __HPP_GRAPHICS__