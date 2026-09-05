#ifndef CHATAPP_COMMON_COLOR_HPP
#define CHATAPP_COMMON_COLOR_HPP

namespace wCommon {

struct Color {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 1.0f;

  void makeDarker(float amount = 0.1f) {
    if (amount < 0.0f)
      amount = 0.0f;
    if (amount > 1.0f)
      amount = 1.0f;
    r *= 1.0f - amount;
    g *= 1.0f - amount;
    b *= 1.0f - amount;
  }

  void makeWhiter(float amount = 0.1f) {
    if (amount < 0.0f)
      amount = 0.0f;
    if (amount > 1.0f)
      amount = 1.0f;
    r += (1.0f - r) * amount;
    g += (1.0f - g) * amount;
    b += (1.0f - b) * amount;
  }
};

} // namespace wCommon

#endif // CHATAPP_COMMON_COLOR_HPP