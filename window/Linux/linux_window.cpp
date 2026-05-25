#include "context.hpp"
#include "window.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <functional>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class LinuxWindow : public wWindow::Window {
public:
  LinuxWindow(int width, int height, std::string title, std::string className) {
    m_width = width;
    m_height = height;
    m_title = std::move(title);

    // Open X11 display
    display = XOpenDisplay(nullptr);
    if (!display) {
      throw std::runtime_error("Failed to open X11 display");
    }

    // Get the default screen
    int screen = DefaultScreen(display);
    ::Window rootWindow = RootWindow(display, screen);

    // Create the window
    window = XCreateSimpleWindow(display, rootWindow, 0, 0, m_width, m_height,
                                 0, BlackPixel(display, screen),
                                 BlackPixel(display, screen));

    if (!window) {
      XCloseDisplay(display);
      throw std::runtime_error("Failed to create X11 window");
    }

    // Set window title
    XStoreName(display, window, m_title.c_str());

    // Register for window close events
    wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteWindow, 1);

    // Select input events
    XSelectInput(display, window,
                 ExposureMask | StructureNotifyMask | KeyPressMask |
                     KeyReleaseMask | PointerMotionMask | ButtonPressMask |
                     ButtonReleaseMask);

    // Map the window to the screen
    XMapWindow(display, window);

    // Create GL context
    glContext = createGLContext(display);
  }

  ~LinuxWindow() override {
    glContext.reset();

    if (window != 0 && display) {
      XDestroyWindow(display, window);
      window = 0;
    }

    if (display) {
      XCloseDisplay(display);
      display = nullptr;
    }
  }

  void pollEvents() override {
    if (!display) {
      return;
    }

    XEvent event;
    while (XPending(display)) {
      XNextEvent(display, &event);

      switch (event.type) {
      case DestroyNotify:
        m_shouldClose = true;
        break;
      case ClientMessage: {
        if (event.xclient.data.l[0] == static_cast<long>(wmDeleteWindow)) {
          m_shouldClose = true;
        }
        break;
      }
      case ConfigureNotify: {
        m_width = event.xconfigure.width;
        m_height = event.xconfigure.height;

        // Call resize callback when window is resized
        renderFrame();
        break;
      }
      default:
        break;
      }
    }
  }

  void swapBuffers() override {
    if (glContext) {
      glContext->swapBuffers();
    }
  }

  bool shouldClose() const override { return m_shouldClose; }
  int getWidth() const override { return m_width; }
  int getHeight() const override { return m_height; }

  void setBackgroundColor(float r, float g, float b, float a) override {
    m_bgColor[0] = r;
    m_bgColor[1] = g;
    m_bgColor[2] = b;
    m_bgColor[3] = a;
  }

  void renderFrame() override {
    glViewport(0, 0, m_width, m_height);
    glClearColor(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    swapBuffers();
  }

private:
  Display *display = nullptr;
  ::Window window = 0;
  Atom wmDeleteWindow = 0;
  std::unique_ptr<wWindow::GLContext> glContext;

  int m_width = 0;
  int m_height = 0;
  std::string m_title;
  bool m_shouldClose = false;

  // Background color (RGBA)
  float m_bgColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
};

std::unique_ptr<wWindow::Window>
createWindow(int width, int height, std::string title, std::string className) {
  return std::make_unique<LinuxWindow>(width, height, std::move(title),
                                       className);
}