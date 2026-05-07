#ifndef __H_CONTEXT__
#define __H_CONTEXT__

#include <memory>

class GLContext {
public:
  virtual ~GLContext() = default;

  virtual void swapBuffers() = 0;
  virtual bool isValid() const = 0;
};

// Factory function - platform-specific implementation
std::unique_ptr<GLContext> createGLContext(void *nativeHandle);

#endif // __H_CONTEXT__