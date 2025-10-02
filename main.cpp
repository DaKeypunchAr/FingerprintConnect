#include "wrapgl_core.hpp" // IWYU pragma: keep
#include <print>

int main(void) {
  std::println("The program just started!");
  GLFW::initialize();
  const Window window = Window::create({800, 600}, "Fingerprint-Connect");

  while (!window.shouldClose()) {
    window.clear({0.3f, 0.3f, 0.3f, 1.0f});

    if (window.isKeyPressed(Key::Escape)) {
      break;
    }

    window.swapBuffers();
    Window::pollEvents();
  }
  std::println("The program just ended!");
}
