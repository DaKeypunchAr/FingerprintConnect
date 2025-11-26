#include "src/font.hpp"
#include "src/font_atlas.hpp"
#include "wrapgl_core.hpp" // IWYU pragma: keep
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <print>

#include "src/text_renderer.hpp"

#include "glad/glad.h"
// Uncomment the below line to let IMGUI run with the program
// #define IMGUI
#include "imgui.hpp" // This is just something that abstracts how i use imgui.

void startLoop(const Window &window);

int main(void) {
  std::println("The program just started!");
  GLFW::initialize();
  const Window window = Window::create({800, 600}, "Fingerprint-Connect");
  imguiInit(window.getRawPointer());

  // The point of this function is to create opengl objects which destroy
  // themselves on their own in another stack so that we can terminate the
  // opengl context after freeing all the opengl objects.
  startLoop(window);

  GLFW::terminate();
  imguiShutdown();
  std::println("The program just ended!");
}

void startLoop(const Window &window) {
  Font font = Font::create("Lato-Regular.ttf");

  FontAtlas atlas = FontAtlas::create(font, 64);
  font.free();

  GL::ShaderProgram shader = GL::ShaderProgram::createFromFolder(
      "/home/dakeypunchar/documents/cpp-projects/fingerprint-connect/shaders");
  // shader.select(); // TODO: Change WrapGL's shaderprogram code to select the
  // //                        uniform while setting uniforms.
  //
  // shader.setUniform("tex", 0);

  // TODO: Make WrapGL enable blending by default and let us ways to control
  // this.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while (!window.shouldClose()) {
    window.clear({0.3f, 0.3f, 0.3f, 1.0f});
    imguiUpdate();

    if (window.isKeyPressed(Key::Escape)) {
      break;
    }

    // TODO: Make WrapGL let us give window size directly.
    int w, h;
    glfwGetWindowSize(window.getRawPointer(), &w, &h);
    TextRenderer::renderText("I'm Da KeypunchAr. Hahaahaa", glm::vec4(0),
                             glm::vec2(100, 100), 1.0f, shader, atlas,
                             glm::uvec2(w, h));

    imguiRender();
    window.swapBuffers();
    Window::pollEvents();
  }
}
