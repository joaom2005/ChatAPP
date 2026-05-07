#include "context.hpp"

#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <GL/glx.h>
#include <X11/Xlib.h>

class LinuxGLContext : public GLContext {
public:
  LinuxGLContext(Display* display, Window window);
  ~LinuxGLContext() override;

  void swapBuffers() override;
  bool isValid() const override;

private:
  void initOpenGL(Display* display, Window window);

  Display* display = nullptr;
  Window window = 0;
  GLXContext glContext = nullptr;
};

LinuxGLContext::LinuxGLContext(Display* display, Window window)
    : display(display), window(window) {
  initOpenGL(display, window);
}

LinuxGLContext::~LinuxGLContext() {
  if (glContext && display) {
    glXMakeCurrent(display, None, nullptr);
    glXDestroyContext(display, glContext);
    glContext = nullptr;
  }
}

void LinuxGLContext::swapBuffers() {
  if (display && window) {
    glXSwapBuffers(display, window);
  }
}

bool LinuxGLContext::isValid() const {
  return glContext != nullptr;
}

void LinuxGLContext::initOpenGL(Display* display, Window window) {
  if (!display || !window) {
    throw std::runtime_error("Invalid X11 display or window");
  }

  // Get a visual that supports OpenGL
  int attribs[] = {
    GLX_RGBA,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER,
    None
  };

  XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attribs);
  if (!vi) {
    throw std::runtime_error("Failed to choose X11 visual for OpenGL");
  }

  GLXContext tempContext = glXCreateContext(display, vi, nullptr, GL_TRUE);
  if (!tempContext) {
    XFree(vi);
    throw std::runtime_error("Failed to create OpenGL context");
  }

  if (!glXMakeCurrent(display, window, tempContext)) {
    glXDestroyContext(display, tempContext);
    XFree(vi);
    throw std::runtime_error("Failed to make OpenGL context current");
  }

  if (!gladLoadGLLoader((GLADloadproc)glXGetProcAddress)) {
    glXMakeCurrent(display, None, nullptr);
    glXDestroyContext(display, tempContext);
    XFree(vi);
    throw std::runtime_error("Failed to initialize GLAD");
  }

  XFree(vi);
  glContext = tempContext;
}

std::unique_ptr<GLContext> createGLContext(void* nativeHandle) {
  // nativeHandle should point to a pair: {Display*, Window}
  // For now, we pass Display* as the handle and Window separately
  // This is a simplified approach - you may need to adjust based on your needs
  Display* display = static_cast<Display*>(nativeHandle);
  if (!display) {
    throw std::runtime_error("Invalid Display pointer");
  }

  // Get the root window as a fallback
  Window window = RootWindow(display, DefaultScreen(display));
  return std::make_unique<LinuxGLContext>(display, window);
}
