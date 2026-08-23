#ifndef CHATAPP_WINDOW_CONTEXT_HPP
#define CHATAPP_WINDOW_CONTEXT_HPP

#include <memory>

namespace wWindow {
class GLContext {
public:
  virtual ~GLContext() = default;

  virtual void swapBuffers()   = 0;
  virtual bool isValid() const = 0;
};

// The nativeHandle is platform specific. See corresponding _context.cpp
std::unique_ptr<wWindow::GLContext> createGLContext(void *nativeHandle);
} // namespace wWindow

#endif // CHATAPP_WINDOW_CONTEXT_HPP