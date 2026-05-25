#include <window.hpp>
#include <input.hpp>
#include <iostream>

// There shouldn't be any ambiguity on main, since only the interfaces are used,
// so we can "using namespace" this
using namespace wWindow;

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