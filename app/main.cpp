#include <iostream>
#include <window.hpp>

#include <glad/glad.h>

int main() {
  std::unique_ptr<Window> window =
      createWindow(800, 400, "ChatAPP", "chat-app");
  glViewport(0, 0, window->getWidth(), window->getHeight());

  // Lambda to render a frame
  auto renderFrame = [&window]() {
    glViewport(0, 0, window->getWidth(), window->getHeight());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window->swapBuffers();
  };

  // Set the callback to be called during resize
  window->setResizeCallback(renderFrame);

  while (!window->shouldClose()) {
    window->pollEvents();

    renderFrame();
  }

  return 0;
}