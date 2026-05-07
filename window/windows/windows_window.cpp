#include "window.hpp"
#include "context.hpp"

#include <dwmapi.h>
// #include <glad/glad.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <windows.h>

class WindowsWindow : public Window {
public:
  WindowsWindow(int width, int height, std::string title,
                std::string className) {
    create(width, height, std::move(title), className);
  }

  ~WindowsWindow() override {
    glContext.reset();

    if (hdc) {
      ReleaseDC(hwnd, hdc);
      hdc = nullptr;
    }

    if (hwnd) {
      DestroyWindow(hwnd);
      hwnd = nullptr;
    }
  }

  void pollEvents() override {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      if (msg.message == WM_QUIT) {
        m_shouldClose = true;
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

protected:
  void create(int width, int height, std::string title,
              std::string className) override {
    m_width = width;
    m_height = height;
    m_title = std::move(title);

    // Register class
    WNDCLASS wc = {};
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    if (wc.hInstance == nullptr) {
      throw std::runtime_error("Failed to get module handle: " +
                               std::to_string(GetLastError()));
    }
    wc.lpszClassName = className.c_str();

    if (!RegisterClass(&wc)) {
      DWORD error = GetLastError();
      if (error != ERROR_CLASS_ALREADY_EXISTS) {
        throw std::runtime_error("Failed to register window class: " +
                                 std::to_string(error));
      }
    }

    hwnd =
        CreateWindowEx(0, className.c_str(), m_title.c_str(),
                       WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width,
                       height, nullptr, nullptr, wc.hInstance, nullptr);

    if (hwnd == nullptr) {
      throw std::runtime_error("Failed to create window: " +
                               std::to_string(GetLastError()));
    }

    applyModernStyle(hwnd);

    ShowWindow(hwnd, SW_SHOW);

    hdc = GetDC(hwnd);
    if (!hdc) {
      throw std::runtime_error("Failed to get device context: " +
                               std::to_string(GetLastError()));
    }

    glContext = createGLContext(hdc);
  }

private:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp,
                                     LPARAM lp) {
    if (msg == WM_DESTROY) {
      PostQuitMessage(0);
      return 0;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
  }

  void applyModernStyle(HWND hwnd) {
    // Dark mode title bar (Windows 10 1809+)
    BOOL darkMode = TRUE;
    HRESULT hr = DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE,
                                       &darkMode, sizeof(darkMode));
    if (FAILED(hr)) {
      // Optional feature, ignore failure
      std::cerr << "Failed to set dark mode: " << hr << std::endl;
    }

    // Rounded corners (Windows 11 only, silently ignored on Win10)
    DWM_WINDOW_CORNER_PREFERENCE corner = DWMWCP_ROUND;
    hr = DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner,
                               sizeof(corner));
    if (FAILED(hr)) {
      // Optional feature, ignore failure
      std::cerr << "Failed to set rounded corners: " << hr << std::endl;
    }
  }

private:
  HWND hwnd = nullptr;
  HDC hdc = nullptr;
  std::unique_ptr<GLContext> glContext;

  int m_width;
  int m_height;

  std::string m_title;

  bool m_shouldClose = false;
};

std::unique_ptr<Window> createWindow(int width, int height, std::string title,
                                     std::string className) {
  return std::make_unique<WindowsWindow>(width, height, std::move(title),
                                         className);
}