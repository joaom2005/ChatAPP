#include <input.hpp>
#include <window.hpp>

int main() {
  auto queue = std::make_shared<wWindow::EventQueue>();
  std::unique_ptr<wWindow::Window> window =
      createWindow(queue, 800, 400, "ChatAPP", "chat-app");
  auto input = std::make_unique<wWindow::Input>(queue);

  window->setBackgroundColor(1.0f, 1.0f, 1.0f, 1.0f);

  while (!window->shouldClose()) {
    window->pollEvents();
    window->renderFrame();

    input->update();

    if (input->isKeyPressed(wWindow::Key::Escape))
      window->forceClose();
  }

  return 0;
}