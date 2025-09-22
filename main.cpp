#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <print>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(void) {
  if (!glfwInit()) {
    std::println("Unable to initialize GLFW!");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  std::println("Initialized GLFW successfully!");
  GLFWwindow *window = glfwCreateWindow(
      800, 600, "Fingerprint Connect - Debug - In progress", nullptr, nullptr);

  if (!window) {
    glfwTerminate();
    std::println("Unable to initialize GLFW Window!");
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::println("Unable to initialize GLAD!");
    return -1;
  }
  std::println("Initialized GLAD successfully!");

  glViewport(0, 0, 800, 600);

  glm::vec4 screen_color = glm::vec4(0.1f, 0.8f, 0.1f, 1.0f);
  glClearColor(screen_color.r, screen_color.g, screen_color.b, screen_color.a);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  std::println("Terminated GLFW successfully!");
}
