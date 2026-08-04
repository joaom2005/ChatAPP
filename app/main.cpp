#include <input.hpp>
#include <renderer.hpp>
#include <window.hpp>

int main() {
  auto queue = std::make_shared<wWindow::EventQueue>();
  std::unique_ptr<wWindow::Window> window =
      createWindow(queue, 800, 400, "ChatAPP", "chat-app");
  auto input = std::make_unique<wWindow::Input>(queue);

  wGraphics::Renderer renderer;
  wGraphics::Color bgColor{1.0f, 1.0f, 1.0f, 1.0f};

  while (!window->shouldClose()) {
    window->pollEvents();

    renderer.beginFrame(window->getWidth(), window->getHeight(), bgColor);

    renderer.drawRect(50, 50, 200, 100, {1.0f, 0.0f, 0.0f, 1.0f});

    renderer.endFrame();

    window->swapBuffers();

    input->update();

    if (input->isKeyPressed(wWindow::Key::Escape))
      window->forceClose();
  }

  return 0;
}