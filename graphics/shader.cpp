#include "shader.hpp"

#include <glad/glad.h>
#include <iostream>
#include <vector>

namespace wGraphics {

static unsigned int compile(unsigned int type, const std::string &src) {
  unsigned int shader = glCreateShader(type);
  const char *src_cstr = src.c_str();
  glShaderSource(shader, 1, &src_cstr, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    int len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> log(len);
    glGetShaderInfoLog(shader, len, nullptr, log.data());
    std::cerr << "Shader compile error: " << log.data() << "\n";
  }
  return shader;
}

Shader::Shader(const std::string &vertSrc, const std::string &fragSrc) {
  unsigned int vs = compile(GL_VERTEX_SHADER, vertSrc);
  unsigned int fs = compile(GL_FRAGMENT_SHADER, fragSrc);

  m_id = glCreateProgram();
  glAttachShader(m_id, vs);
  glAttachShader(m_id, fs);
  glLinkProgram(m_id);

  int success;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success) {
    int len;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> log(len);
    glGetProgramInfoLog(m_id, len, nullptr, log.data());
    std::cerr << "Shader link error: " << log.data() << "\n";
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
}

Shader::~Shader() { glDeleteProgram(m_id); }

void Shader::use() const { glUseProgram(m_id); }

void Shader::setMat4(const char *name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE,
                     glm::value_ptr(mat));
}

void Shader::setVec4(const char *name, float x, float y, float z,
                     float w) const {
  glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}

void Shader::setInt(const char *name, int value) const {
  glUniform1i(glGetUniformLocation(m_id, name), value);
}

} // namespace wGraphics