#include "context.hpp"
#include <windows.h>

#include <glad/glad.h>
#include <memory>
#include <stdexcept>
#include <string>

class WindowsGLContext : public GLContext {
public:
  WindowsGLContext(HDC hdc);
  ~WindowsGLContext() override;

  void swapBuffers() override;
  bool isValid() const override;

private:
  void initOpenGL(HDC hdc);

  HGLRC glContext = nullptr;
  HDC deviceContext = nullptr;
};

WindowsGLContext::WindowsGLContext(HDC hdc) : deviceContext(hdc) {
  initOpenGL(hdc);
}

WindowsGLContext::~WindowsGLContext() {
  if (glContext) {
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(glContext);
    glContext = nullptr;
  }
}

void WindowsGLContext::swapBuffers() {
  if (deviceContext) {
    ::SwapBuffers(deviceContext);
  }
}

bool WindowsGLContext::isValid() const { return glContext != nullptr; }

void WindowsGLContext::initOpenGL(HDC hdc) {
  if (!hdc) {
    throw std::runtime_error("Invalid device context");
  }

  PIXELFORMATDESCRIPTOR pfd = {};
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;
  pfd.iLayerType = PFD_MAIN_PLANE;

  int pixelFormat = ChoosePixelFormat(hdc, &pfd);
  if (pixelFormat == 0) {
    throw std::runtime_error("Failed to choose pixel format: " +
                             std::to_string(GetLastError()));
  }

  if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
    throw std::runtime_error("Failed to set pixel format: " +
                             std::to_string(GetLastError()));
  }

  HGLRC tempContext = wglCreateContext(hdc);
  if (!tempContext) {
    throw std::runtime_error("Failed to create temporary OpenGL context: " +
                             std::to_string(GetLastError()));
  }

  if (!wglMakeCurrent(hdc, tempContext)) {
    wglDeleteContext(tempContext);
    throw std::runtime_error(
        "Failed to make temporary OpenGL context current: " +
        std::to_string(GetLastError()));
  }

  if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) {
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tempContext);
    throw std::runtime_error("Failed to initialize GLAD");
  }

  glContext = tempContext;
}

std::unique_ptr<GLContext> createGLContext(void *nativeHandle) {
  return std::make_unique<WindowsGLContext>(static_cast<HDC>(nativeHandle));
}
