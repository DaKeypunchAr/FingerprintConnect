#include "layout.hpp"
#include "src/font_atlas.hpp"
#include "src/text_renderer.hpp"
#include "wrapgl/shader_program.hpp"
#include "wrapgl/shapes.hpp"
#include "wrapgl/vertex_array.hpp"
#include "wrapgl/window.hpp"
#include <fstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <print>
#include <string_view>

Layout::Config Layout::loadConfig(const std::filesystem::path &configPath) {
  std::ifstream fileReader(configPath);
  if (!fileReader.is_open()) {
    std::println(stderr, "Unable to open the config at {0}",
                 configPath.string());
  }
  std::string path;
  std::getline(fileReader, path);

  std::array<glm::vec2, 26> offsets;
  unsigned int idx = 0;
  for (std::string line; std::getline(fileReader, line); idx++) {
    unsigned int space_idx = line.find(' ');
    offsets[idx].x = std::stof(line.substr(0, space_idx));
    offsets[idx].y = std::stof(line.substr(space_idx + 1));
  }

  return Layout::Config{
      .texPath = path, .offsets = offsets, .path = configPath};
}

void Layout::saveConfig() const {
  std::ofstream fileWriter(m_ConfigPath);
  if (!fileWriter.is_open()) {
    std::println(stderr, "Unable to open the config at {0} to save it",
                 m_ConfigPath.string());
  }

  fileWriter << m_TexturePath.string();
  for (const glm::vec2 offset : m_OffsetOfKeys) {
    fileWriter << '\n' << offset.x << ' ' << offset.y;
  }
}

Layout::Layout(const std::filesystem::path &configPath)
    : Layout(loadConfig(configPath)) {}

Layout::Layout(const Config &config)
    : Layout(config.texPath, config.offsets, config.path) {}

Layout::Layout(const std::filesystem::path &texturePath,
               const std::array<glm::vec2, 26> &offsetOfKeys,
               const std::filesystem::path &path)
    : m_Reference(GL::Texture2D::loadFromFile(texturePath)), m_RefScale(1.5F),
      m_RefPos((glm::vec2(Window::getActiveWindow()->getCurrentDimension()) -
                glm::vec2(m_Reference.getDimension()) * m_RefScale) /
               2.0F),
      m_RefVA(GL::VertexArray::create()), m_PointVA(GL::VertexArray::create()),
      m_RefPosVB(GL::VertexBuffer::createAndAllocate(
          Rectangle(m_RefPos,
                    glm::vec2(m_Reference.getDimension()) * m_RefScale)
              .inBuffer(),
          GL::BufferUpdateFrequency::DYNAMIC)),
      m_RefTexVB(GL::VertexBuffer::createAndAllocate({0, 0, 1, 0, 0, 1, 1, 1})),
      m_TexShader(GL::ShaderProgram::createTextureShader()),
      m_PointShader(GL::ShaderProgram::createFromFolder("shaders/PointShader")),
      m_ShowKeys(false), m_OffsetOfKeys(offsetOfKeys), m_ConfigPath(path),
      m_TexturePath(texturePath) {
  m_RefVA.setFormat({GL::Attribute::create(GL::DataType::F32, 2, 0),
                     GL::Attribute::create(GL::DataType::F32, 2, 1)});
  m_RefVA.setBufferBindings(
      {GL::Binding::create(m_RefPosVB, 0, 0, sizeof(float) * 2),
       GL::Binding::create(m_RefTexVB, 1, 0, sizeof(float) * 2)});
  m_PointVA.setFormat({});
  m_PointVA.setBufferBindings({});
}

void Layout::render(const FontAtlas &atlas,
                    const GL::ShaderProgram &textShader) {
  if (m_ShowReference) {
    m_RefVA.select();
    m_TexShader.setUniform(
        "uProjection",
        glm::ortho(
            0.0F,
            static_cast<float>(Window::getActiveWindow()->getCurrentWidth()),
            0.0F,
            static_cast<float>(Window::getActiveWindow()->getCurrentHeight())));
    m_TexShader.setUniform("uTexture", 0);
    m_Reference.bind(0);
    GL::Renderer::drawArrays(GL::RenderMode::TRIANGLE_STRIP, 4);
  }

  if (!m_ShowKeys)
    return;

  m_PointShader.setUniform("uColor", glm::vec4(1.0f, 0.4f, 0.3f, 1.0f));
  m_PointShader.setUniform("uRadius", 5.0f);
  std::string c = "A";
  for (glm::vec2 point : m_OffsetOfKeys) {
    point += m_RefPos;
    TextRenderer::renderText(c, glm::vec4(1.0f, 0.4f, 0.3f, 1.0f),
                             point + glm::vec2(-10, 5), 0.5F, textShader,
                             atlas);
    c[0]++;
    m_PointVA.select();
    m_PointShader.setUniform("uPoint", point);
    GL::Renderer::drawArrays(GL::RenderMode::TRIANGLE_STRIP, 4);
  }
}

void Layout::update() {
  if (Window::getActiveWindow()->isKeyPressed(Key::LeftControl) &&
      Window::getActiveWindow()->isKeyPressed(Key::S)) {
    saveConfig();
  }
  if (!m_OffsetEditMode)
    return;

  static bool lockedD = false, lockedA = false;
  if (Window::getActiveWindow()->isKeyPressed(Key::LeftControl)) {
    if (Window::getActiveWindow()->isKeyPressed(Key::D)) {
      if (!lockedD) {
        m_EditModeSelectedChar++;
        if (m_EditModeSelectedChar == m_OffsetOfKeys.size()) {
          m_EditModeSelectedChar = 0;
        }
        lockedD = true;
      }
    } else {
      lockedD = false;
    }
    if (Window::getActiveWindow()->isKeyPressed(Key::A)) {
      if (!lockedA) {
        m_EditModeSelectedChar--;
        if (m_EditModeSelectedChar == 255) {
          m_EditModeSelectedChar = m_OffsetOfKeys.size() - 1;
        }
        lockedA = true;
      }
    } else {
      lockedA = false;
    }
  }

  if (Window::getActiveWindow()->isMousePressed(MouseButton::LEFT)) {
    glm::vec2 pos = Window::getActiveWindow()->getCursorPosition();
    pos.y = Window::getActiveWindow()->getCurrentHeight() - pos.y;
    m_OffsetOfKeys[m_EditModeSelectedChar] = pos - m_RefPos;
  }
}

void Layout::updateBuffer() const {
  m_RefPosVB.update(
      Rectangle(m_RefPos, glm::vec2(m_Reference.getDimension()) * m_RefScale)
          .inBuffer());
}
