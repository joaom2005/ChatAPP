#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <memory>
#include <string>

class Window {
protected:
  virtual void create(int width, int height, std::string title,
                      std::string className) = 0;

public:
  virtual ~Window() = default;

  virtual void pollEvents() = 0;
  virtual void swapBuffers() = 0;

public:
  virtual bool shouldClose() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
};

std::unique_ptr<Window> createWindow(int width, int height, std::string title,
                                     std::string className);

#endif // _WINDOW_HPP