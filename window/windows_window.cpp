#include "window.hpp"

#include <dwmapi.h>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <windows.h>

class WindowsWindow : public Window {
public:
  WindowsWindow(int width, int height, std::string title,
                std::string className) {
    create(width, height, std::move(title), className);
  }

  ~WindowsWindow() override {
    if (glContext) {
      wglMakeCurrent(nullptr, nullptr);
      wglDeleteContext(glContext);
      glContext = nullptr;
    }

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
    if (hdc) {
      ::SwapBuffers(hdc);
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
      // If already exists, continue (assuming it's the same)
    }

    hwnd = CreateWindowEx(
        0, "MyWindowClass", m_title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
        CW_USEDEFAULT, width, height, nullptr, nullptr, wc.hInstance, nullptr);

    if (hwnd == nullptr) {
      throw std::runtime_error("Failed to create window: " +
                               std::to_string(GetLastError()));
    }

    applyModernStyle(hwnd);

    ShowWindow(hwnd, SW_SHOW);

    initOpenGL();
  }

private:
  void initOpenGL() {
    hdc = GetDC(hwnd);
    if (!hdc) {
      throw std::runtime_error("Failed to get device context: " +
                               std::to_string(GetLastError()));
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0) {
      throw std::runtime_error("Failed to choose pixel format: " +
                               std::to_string(GetLastError()));
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
      throw std::runtime_error("Failed to set pixel format: " +
                               std::to_string(GetLastError()));
    }

    HGLRC tempContext = wglCreateContext(hdc);
    if (!tempContext) {
      throw std::runtime_error("Failed to create temporary OpenGL context: " +
                               std::to_string(GetLastError()));
    }

    if (!wglMakeCurrent(hdc, tempContext)) {
      wglDeleteContext(tempContext);
      throw std::runtime_error(
          "Failed to make temporary OpenGL context current: " +
          std::to_string(GetLastError()));
    }

    if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) {
      wglMakeCurrent(nullptr, nullptr);
      wglDeleteContext(tempContext);
      throw std::runtime_error("Failed to initialize GLAD");
    }

    glContext = tempContext;
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
  HGLRC glContext = nullptr;

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