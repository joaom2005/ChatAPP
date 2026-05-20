#ifndef __HPP_WINDOW__
#define __HPP_WINDOW__

#include "event.hpp"

#include <functional>
#include <memory>
#include <string>

class Window {
public:
  virtual ~Window() = default;

  virtual void pollEvents() = 0;
  virtual void swapBuffers() = 0;

  // Set the background color for frame rendering
  virtual void setBackgroundColor(float r, float g, float b, float a) = 0;

  // Render a frame with the background color and swap buffers
  virtual void renderFrame() = 0;

  // Close the window from outside
  virtual void forceClose() = 0;

public:
  virtual bool shouldClose() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
};

std::unique_ptr<Window> createWindow(std::shared_ptr<EventQueue> eventQueue,
                                     int width, int height, std::string title,
                                     std::string className);

#endif // __HPP_WINDOW__