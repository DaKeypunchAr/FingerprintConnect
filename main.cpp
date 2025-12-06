#include "glm/gtc/matrix_transform.hpp" // IWYU pragma: keep
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "src/font_atlas.hpp"
#include "src/layout.hpp"
#include "src/text_renderer.hpp"
#include "wrapgl/renderer.hpp"
#include "wrapgl/shader_program.hpp"
#include "wrapgl/vertex_array.hpp"
#include "wrapgl_core.hpp" // IWYU pragma: keep
#include <memory>
#include <print>
#include <stdexcept>

#include "glad/glad.h"

void constructImGuiLayout();

// Uncomment the below line to let IMGUI run with the program
#define IMGUI
#include "imgui.hpp" // This is just something that abstracts how i use imgui.

std::unique_ptr<Layout> layout;
std::string text;

void constructImGuiLayout() {
  ImGui::Begin("Settings");
  bool update = false;

  ImGui::InputText("Text", &text);

  float scale = layout->getScale();
  ImGui::DragFloat("Scale", &scale, 0.01f);
  if (layout->getScale() != scale) {
    layout->setScale(scale);
    update = true;
  }

  glm::vec2 pos = layout->getPos();
  ImGui::DragFloat2("Position", &pos.x);

  if (layout->getPos() != pos) {
    layout->setPos(pos);
    update = true;
  }

  bool showKeys = layout->getShowKeys();
  ImGui::Checkbox("Show Keys", &showKeys);
  if (showKeys != layout->getShowKeys()) {
    layout->setShowKeys(showKeys);
  }

  bool showReference = layout->getShowReference();
  ImGui::Checkbox("Show Reference", &showReference);
  if (showReference != layout->getShowReference()) {
    layout->setShowReference(showReference);
  }

  bool offEditMode = layout->getOffsetEditMode();
  ImGui::Checkbox("Offset Edit Mode", &offEditMode);
  if (offEditMode != layout->getOffsetEditMode()) {
    layout->setOffsetEditMode(offEditMode);
  }

  if (offEditMode) {
    int idx = layout->getEditModeSelectedChar();
    ImGui::InputInt("Selected Char Index", &idx);
    if (idx != layout->getEditModeSelectedChar()) {
      layout->setEditModeSelectedChar(idx);
    }
  }

  ImGui::End();

  if (update) {
    layout->updateBuffer();
  }

  ImGui::ShowDemoWindow();
}

enum class TextDesignMode { POINT, SEGMENT };
void renderDesign(const TextDesignMode mode);

int main(void) {
  std::println("The program just started!");
  const Window window = Window::create({800, 600}, "Fingerprint-Connect");
  imguiInit(window.getRawPointer());
  GL::Renderer::enableCapability(GL::Capability::BLENDING);
  GL::Renderer::setBlendFactors(GL::BlendFactor::SOURCE_ALPHA,
                                GL::BlendFactor::ONE_MINUS_SOURCE_ALPHA);

  STB::setFlipVerticallyOnLoad(true);

  layout = std::make_unique<Layout>("qwerty.kl");

  GL::ShaderProgram textProgram =
      GL::ShaderProgram::createFromFolder("shaders/FontShader");
  FontAtlas textAtlas = FontAtlas::create(Font::create("Lato-Regular.ttf"), 32);

  TextDesignMode mode = TextDesignMode::SEGMENT;

  GL::Renderer::setClearColor({0.3f, 0.3f, 0.3f});
  while (!window.shouldClose()) {
    GL::Renderer::clear();
    imguiUpdate();
    layout->update();

    if (window.isKeyPressed(Key::Escape)) {
      break;
    }

    renderDesign(mode);
    TextRenderer::renderText(text, glm::vec4(0.5f, 0.8f, 0.3f, 1.0f),
                             glm::vec2(500, 100), 1.0F, textProgram, textAtlas);

    layout->render(textAtlas, textProgram);

    imguiRender();
    window.swapBuffers();
    Window::pollEvents();
  }
  imguiShutdown();
  std::println("The program just ended!");
}

void renderDesign(const TextDesignMode mode) {
  static GL::ShaderProgram pointShader =
      GL::ShaderProgram::createFromFolder("shaders/PointShader");
  static GL::ShaderProgram segmentShader =
      GL::ShaderProgram::createFromFolder("shaders/SegmentShader");
  static GL::VertexArray pointVA = GL::VertexArray::create();
  int chars = 0;
  for (char c : text) {
    if (c >= 'a' && c <= 'z')
      c = c - 'a' + 'A';
    if (c >= 'A' && c <= 'Z')
      chars |= 1 << (c - 'A');
  }

  switch (mode) {
  case TextDesignMode::POINT: {
    pointShader.select();
    pointShader.setUniform(
        "uProjection",
        glm::ortho(
            0.0F,
            static_cast<float>(Window::getActiveWindow()->getCurrentWidth()),
            0.0F,
            static_cast<float>(Window::getActiveWindow()->getCurrentHeight())));
    pointShader.setUniform("uColor", glm::vec4(0.3f, 0.9f, 0.2f, 1.0f));
    pointShader.setUniform("uRadius", 4.0F);
    pointVA.select();
    for (unsigned int i = 0; i < 26; i++) {
      bool toRender = ((chars >> i) & 1) == 1;
      if (toRender) {
        pointShader.setUniform("uPoint",
                               layout->getOffset(i) + layout->getPos());
        GL::Renderer::drawArrays(GL::RenderMode::TRIANGLE_STRIP, 4);
      }
    }
  } break;
  case TextDesignMode::SEGMENT: {
    segmentShader.select();
    segmentShader.setUniform(
        "uProjection",
        glm::ortho(
            0.0F,
            static_cast<float>(Window::getActiveWindow()->getCurrentWidth()),
            0.0F,
            static_cast<float>(Window::getActiveWindow()->getCurrentHeight())));
    segmentShader.setUniform("uColor", glm::vec4(0.3f, 0.9f, 0.2f, 1.0f));
    segmentShader.setUniform("uWidth", 1.0F);
    pointVA.select();
    for (unsigned int i = 0; i < 26; i++) {
      bool toRender = ((chars >> i) & 1) == 1;
      if (toRender) {
        segmentShader.setUniform("uStart",
                                 layout->getOffset(i) + layout->getPos());

        for (unsigned int j = 0; j < 26; j++) {
          if (i == j)
            continue;
          bool toRender2 = ((chars >> j) & 1) == 1;
          if (toRender2) {
            segmentShader.setUniform("uEnd",
                                     layout->getOffset(j) + layout->getPos());
            GL::Renderer::drawArrays(GL::RenderMode::TRIANGLE_STRIP, 4);
          }
        }
      }
    }
  } break;
  default:
    throw std::runtime_error("Unknown design mode!");
  }
}
