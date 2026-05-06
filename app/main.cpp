#include <iostream>
#include <window.hpp>

#include <glad/glad.h>

int main() {
  std::unique_ptr<Window> window = createWindow(800, 400, "ChatAPP", "chatapp");

  glViewport(0, 0, window->getWidth(), window->getHeight());

  while (!window->shouldClose()) {
    window->pollEvents();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window->swapBuffers();
  }

  return 0;
}