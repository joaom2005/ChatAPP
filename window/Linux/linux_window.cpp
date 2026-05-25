#include "context.hpp"
#include "window.hpp"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <functional>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

class LinuxWindow : public wWindow::Window {
public:
  LinuxWindow(
      std::shared_ptr<wWindow::EventQueue> eventQueue, int width, int height,
      std::string title, std::string className
  ) {
    m_eventQueue = eventQueue;
    m_width      = width;
    m_height     = height;
    m_title      = std::move(title);

    // Open X11 display
    display = XOpenDisplay(nullptr);
    if (!display) {
      throw std::runtime_error("Failed to open X11 display");
    }

    // Get the default screen
    int screen          = DefaultScreen(display);
    ::Window rootWindow = RootWindow(display, screen);

    // Create the window
    window = XCreateSimpleWindow(
        display, rootWindow, 0, 0, m_width, m_height, 0,
        BlackPixel(display, screen), BlackPixel(display, screen)
    );

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
    XSelectInput(
        display, window,
        ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask |
            PointerMotionMask | ButtonPressMask | ButtonReleaseMask
    );

    // Map the window to the screen
    XMapWindow(display, window);

    // Create GL context to send
    GLContextCreateInfo info{
        display, window
    }; // On stack because I only pass this once
    glContext = createGLContext(&info);
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
        m_width  = event.xconfigure.width;
        m_height = event.xconfigure.height;

        // Call resize callback when window is resized
        renderFrame();
        break;
      }
      case KeyPress: {
        KeySym keysym = XLookupKeysym(&event.xkey, 0);
        if (auto key = translateKey(keysym)) {
          m_eventQueue->push(wWindow::KeyEvent{*key, true});
        }
        break;
      }
      case KeyRelease: {
        KeySym keysym = XLookupKeysym(&event.xkey, 0);
        if (auto key = translateKey(keysym)) {
          m_eventQueue->push(wWindow::KeyEvent{*key, false});
        }
        break;
      }
      case MotionNotify:
        m_eventQueue->push(
            wWindow::MouseMove{event.xmotion.x, event.xmotion.y}
        );
        break;
      case ButtonPress: {
        if (auto key = translateMouseButton(event.xbutton.button)) {
          m_eventQueue->push(wWindow::KeyEvent{*key, true});
        }
        break;
      }
      case ButtonRelease: {
        if (auto key = translateMouseButton(event.xbutton.button)) {
          m_eventQueue->push(wWindow::KeyEvent{*key, false});
        }
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

  void forceClose() { m_shouldClose = true; }

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
  static std::optional<wWindow::Key> translateKey(KeySym keysym) {
    using namespace wWindow;
    switch (keysym) {
    case XK_w:
    case XK_W:
      return Key::W;
    case XK_a:
    case XK_A:
      return Key::A;
    case XK_s:
    case XK_S:
      return Key::S;
    case XK_d:
    case XK_D:
      return Key::D;
    case XK_Escape:
      return Key::Escape;
    default:
      return std::nullopt;
    }
  }

  static std::optional<wWindow::Key> translateMouseButton(unsigned int button) {
    using namespace wWindow;
    switch (button) {
    case Button1:
      return Key::MouseLeftButton;
    case Button2:
      return Key::MouseMiddleButton;
    case Button3:
      return Key::MouseRightButton;
    default:
      return std::nullopt;
    }
  }

private:
  Display *display    = nullptr;
  ::Window window     = 0;
  Atom wmDeleteWindow = 0;
  std::unique_ptr<wWindow::GLContext> glContext;
  std::shared_ptr<wWindow::EventQueue> m_eventQueue;

  int m_width  = 0;
  int m_height = 0;
  std::string m_title;
  bool m_shouldClose = false;

  // Background color (RGBA)
  float m_bgColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
};

std::unique_ptr<wWindow::Window> wWindow::createWindow(
    std::shared_ptr<wWindow::EventQueue> eventQueue, int width, int height,
    std::string title, std::string className
) {
  return std::make_unique<LinuxWindow>(
      eventQueue, width, height, std::move(title), className
  );
}