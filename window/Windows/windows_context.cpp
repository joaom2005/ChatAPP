#include "context.hpp"
#include <windows.h>

#include <glad/glad.h>
#include <memory>
#include <stdexcept>
#include <string>

class WindowsGLContext : public wWindow::GLContext {
public:
  WindowsGLContext(HDC hdc) : deviceContext(hdc) {
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

  ~WindowsGLContext() override {
    if (glContext) {
      wglMakeCurrent(nullptr, nullptr);
      wglDeleteContext(glContext);
      glContext = nullptr;
    }
  }

  void swapBuffers() override {
    if (deviceContext) {
      ::SwapBuffers(deviceContext);
    }
  }
  bool isValid() const override { return glContext != nullptr; }

private:
  HGLRC glContext = nullptr;
  HDC deviceContext = nullptr;
};

std::unique_ptr<wWindow::GLContext>
wWindow::createGLContext(void *nativeHandle) {
  return std::make_unique<WindowsGLContext>(static_cast<HDC>(nativeHandle));
}
