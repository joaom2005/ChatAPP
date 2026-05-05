#include <iostream>
#include <window.hpp>

#include <glad/glad.h>

int main() {
  std::unique_ptr<Window> window = createWindow(800, 400, "Title");

  while (!window->shouldClose()) {
    window->pollEvents();

    window->swapBuffers();
  }

  return 0;
}