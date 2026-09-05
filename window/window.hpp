#ifndef CHATAPP_WINDOW_WINDOW_HPP
#define CHATAPP_WINDOW_WINDOW_HPP

#include "cursor.hpp"
#include "event.hpp"

#include <functional>
#include <memory>
#include <string>

#ifdef __linux__
#include <X11/Xlib.h>
struct GLContextCreateInfo {
  Display *display;
  ::Window window;
};
#endif

namespace wWindow {

class Window {
public:
  virtual ~Window() = default;

  virtual void pollEvents() = 0;
  virtual void swapBuffers() = 0;
  virtual void setRenderCallback(std::function<void()> callback) = 0;
  virtual void forceClose() = 0;

  virtual bool shouldClose() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
  virtual void setCursor(wCommon::CursorType cursor) = 0; // NEW
};

std::unique_ptr<wWindow::Window>
createWindow(std::shared_ptr<wWindow::EventQueue> eventQueue, int width,
             int height, std::string title, std::string className);
} // namespace wWindow

#endif // CHATAPP_WINDOW_WINDOW_HPP