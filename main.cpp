#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "wrapgl_core.hpp" // IWYU pragma: keep
#include <print>

void imguiInit(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  ImGui_ImplGlfw_InitForOpenGL(
      window, true); // Second param install_callback=true will install
                     // GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();
}

void imguiUpdate() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();
}

void imguiRender() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imguiShutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

int main(void) {
  std::println("The program just started!");
  GLFW::initialize();
  const Window window = Window::create({800, 600}, "Fingerprint-Connect");
  imguiInit(window.getRawPointer());

  while (!window.shouldClose()) {
    window.clear({0.3f, 0.3f, 0.3f, 1.0f});
    imguiUpdate();

    if (window.isKeyPressed(Key::Escape)) {
      break;
    }

    imguiRender();
    window.swapBuffers();
    Window::pollEvents();
  }

  imguiShutdown();
  std::println("The program just ended!");
}
