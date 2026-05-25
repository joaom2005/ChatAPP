#ifndef __HPP_CONTEXT__
#define __HPP_CONTEXT__

#include <memory>

namespace wWindow {
class GLContext {
public:
  virtual ~GLContext() = default;

  virtual void swapBuffers() = 0;
  virtual bool isValid() const = 0;
};
} // namespace wWindow

std::unique_ptr<wWindow::GLContext> createGLContext(void *nativeHandle);

#endif // __HPP_CONTEXT__