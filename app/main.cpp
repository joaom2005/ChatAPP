#include <input.hpp>
#include <iostream>
#include <window.hpp>

int main() {
  auto queue = std::make_shared<EventQueue>();
  std::unique_ptr<Window> window =
      createWindow(queue, 800, 400, "ChatAPP", "chat-app");
  auto input = std::make_unique<Input>(queue);

  window->setBackgroundColor(1.0f, 1.0f, 1.0f, 1.0f);

  while (!window->shouldClose()) {
    window->pollEvents();
    window->renderFrame();

    input->update();

    if (input->isKeyPressed(Key::Escape))
      window->forceClose();
  }

  return 0;
}