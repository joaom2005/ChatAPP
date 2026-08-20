#include <input.hpp>
#include <renderer.hpp>
#include <window.hpp>
#include <filesystem>

int main(int argc, char **argv) {
  auto queue = std::make_shared<wWindow::EventQueue>();
  std::unique_ptr<wWindow::Window> window =
      createWindow(queue, 800, 400, "ChatAPP", "chat-app");
  auto input = std::make_unique<wWindow::Input>(queue);

  wGraphics::Renderer renderer;
  wGraphics::Color bgColor{1.0f, 1.0f, 1.0f, 1.0f};

  auto executableDir = std::filesystem::absolute(argv[0]).parent_path();
  auto fontPath      = executableDir / "assets" / "fonts" / "arial.ttf";
  wGraphics::Font Arial(fontPath.string(), 12);

  auto renderFrame = [&]() {
    renderer.beginFrame(window->getWidth(), window->getHeight(), bgColor);
    renderer.drawRect(50, 50, 200, 100, {1.0f, 0.0f, 0.0f, 1.0f});
    renderer.drawText(
        250, 100, "Hello, World!", Arial,
        wGraphics::Color{0.5f, 0.5f, 0.5f, 1.0f}
    );
    renderer.endFrame();
    window->swapBuffers();
  };

  window->setRenderCallback(renderFrame);

  while (!window->shouldClose()) {
    window->pollEvents();

    renderFrame();

    input->update();

    if (input->isKeyPressed(wWindow::Key::Escape))
      window->forceClose();
  }

  return 0;
}