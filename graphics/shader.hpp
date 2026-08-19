#pragma once
#include <gtc/type_ptr.hpp>
#include <string>

namespace wGraphics {

class Shader {
public:
  Shader(const std::string &vertSrc, const std::string &fragSrc);
  ~Shader();

  void use() const;
  void setMat4(const char *name, const glm::mat4 &mat) const;
  void setVec4(const char *name, float x, float y, float z, float w) const;
  void setInt(const char *name, int value) const;

private:
  unsigned int m_id;
};

} // namespace wGraphics