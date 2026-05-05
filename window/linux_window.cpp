#include "window.hpp"
#include <iostream>

class LinuxWindow : public Window {
public:
    void create(int width, int height, const char* title) override {
        std::cout << "Linux window created\n";
    }

    void pollEvents() override {}
    void swapBuffers() override {}
};

// Factory function
Window* createWindow() {
    return new LinuxWindow();
}