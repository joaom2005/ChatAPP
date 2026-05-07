#ifndef __HPP_WINDOW__
#define __HPP_WINDOW__

#include <memory>
#include <string>
#include <functional>

class Window {
protected:
  virtual void create(int width, int height, std::string title,
                      std::string className) = 0;

public:
  virtual ~Window() = default;

  virtual void pollEvents() = 0;
  virtual void swapBuffers() = 0;
  
  // Set a callback to be called during window resizing/moving
  virtual void setResizeCallback(std::function<void()> callback) = 0;

public:
  virtual bool shouldClose() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
};

std::unique_ptr<Window> createWindow(int width, int height, std::string title,
                                     std::string className);

#endif // __HPP_WINDOW__