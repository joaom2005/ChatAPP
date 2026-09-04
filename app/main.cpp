#include <UIManager.hpp>
#include <filesystem>
#include <input.hpp>
#include <renderer.hpp>
#include <window.hpp>

#include <widgets/button_widget.hpp>
#include <widgets/container_widget.hpp>

int main(int argc, char **argv) {
  auto queue = std::make_shared<wWindow::EventQueue>();
  std::unique_ptr<wWindow::Window> window =
      createWindow(queue, 800, 400, "ChatAPP", "chat-app");
  auto input = std::make_unique<wWindow::Input>(queue);

  wGraphics::Renderer renderer{};
  wGraphics::Color bgColor{1.0f, 1.0f, 1.0f, 1.0f};

  auto executableDir = std::filesystem::absolute(argv[0]).parent_path();
  auto fontPath = executableDir / "assets" / "fonts" / "arial.ttf";
  wGraphics::Font Arial(fontPath.string(), 32);

  auto root = std::make_unique<wWidget::ContainerWidget>();
  root->setX(root->getX() + 100);
  root->addChild(std::make_unique<wWidget::ButtonWidget>(
      250.0f, 100.0f, 200.0f, 100.0f, wGraphics::Color{0.5f, 0.5f, 0.5f, 1.0f},
      "Click me", Arial, wGraphics::Color{1.0f, 0.0f, 0.0f, 1.0f}));

  auto renderFrame = [&]() {
    renderer.beginFrame(window->getWidth(), window->getHeight(), bgColor);
    root->draw(renderer);
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

// Disable sanizer check for nvidea library (cuz I can't do nothing about these
// :p)
extern "C" const char *__lsan_default_suppressions() {
  return "leak:libnvidia-glcore.so\n"
         "leak:libnvidia-glsi.so\n";
}