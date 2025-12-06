#include "text_renderer.hpp"
#include "wrapgl/shader_program.hpp"
#include "wrapgl_core.hpp" // IWYU pragma: keep
#include <glm/ext/matrix_clip_space.hpp>

void TextRenderer::renderText(const std::string_view text,
                              const glm::vec4 color, const glm::vec2 pos,
                              const float scale,
                              const GL::ShaderProgram &program,
                              const FontAtlas &atlas) {
  // TODO: Use a uniform buffer
  GL::VertexArray va = GL::VertexArray::create();
  std::vector<float> posBuffer(text.size() * 4 * 2);
  std::vector<float> texCoordBuffer(text.size() * 4 * 2);
  std::vector<unsigned char> indexBuffer(text.size() * 6);

  glm::vec2 pen = pos;
  for (unsigned int i = 0; i < text.size(); i++) {
    char c = text.at(i);
    if (c == ' ') {
      pen += atlas.getAdvanceForChar(' ') * scale;
      continue;
    }
    glm::vec2 dim = glm::vec2(atlas.getDimensionForChar(c)) * scale;
    glm::vec2 bearing = atlas.getBearingForChar(c) * scale;

    glm::vec4 bltr = glm::vec4(pen.x + bearing.x, pen.y + bearing.y - dim.y,
                               pen.x + bearing.x + dim.x, pen.y + bearing.y);

    posBuffer[8 * i + 0] = bltr.x;
    posBuffer[8 * i + 1] = bltr.y;
    posBuffer[8 * i + 2] = bltr.z;
    posBuffer[8 * i + 3] = bltr.y;
    posBuffer[8 * i + 4] = bltr.x;
    posBuffer[8 * i + 5] = bltr.w;
    posBuffer[8 * i + 6] = bltr.z;
    posBuffer[8 * i + 7] = bltr.w;

    glm::vec4 texCoord = atlas.getTexCoordRectForChar(c);

    texCoordBuffer[8 * i + 0] = texCoord.x;
    texCoordBuffer[8 * i + 1] = texCoord.y;
    texCoordBuffer[8 * i + 2] = texCoord.z;
    texCoordBuffer[8 * i + 3] = texCoord.y;
    texCoordBuffer[8 * i + 4] = texCoord.x;
    texCoordBuffer[8 * i + 5] = texCoord.w;
    texCoordBuffer[8 * i + 6] = texCoord.z;
    texCoordBuffer[8 * i + 7] = texCoord.w;

    indexBuffer[6 * i + 0] = 4 * i;
    indexBuffer[6 * i + 1] = 4 * i + 1;
    indexBuffer[6 * i + 2] = 4 * i + 2;
    indexBuffer[6 * i + 3] = 4 * i + 2;
    indexBuffer[6 * i + 4] = 4 * i + 1;
    indexBuffer[6 * i + 5] = 4 * i + 3;

    glm::vec2 advance = atlas.getAdvanceForChar(c) * scale;
    pen += advance;
  }

  GL::VertexBuffer posVb = GL::VertexBuffer::createAndAllocate(posBuffer);
  GL::VertexBuffer texVb = GL::VertexBuffer::createAndAllocate(texCoordBuffer);
  GL::IndexBuffer ib = GL::IndexBuffer::createAndAllocate(indexBuffer);

  va.linkIndexBuffer(ib);
  va.setBufferBindings({GL::Binding::create(posVb, 0, 0, sizeof(float) * 2),
                        GL::Binding::create(texVb, 1, 0, sizeof(float) * 2)});
  va.setFormat({GL::Attribute::create(GL::DataType::F32, 2, 0),
                GL::Attribute::create(GL::DataType::F32, 2, 1)});
  va.select();

  atlas.bindTextureAt(0);
  va.select();
  program.select();
  program.setUniform("uTexture", 0);
  program.setUniform("uColor", color);
  program.setUniform(
      "uProjection",
      glm::ortho(
          0.0f,
          static_cast<float>(Window::getActiveWindow()->getCurrentWidth()),
          0.0f,
          static_cast<float>(Window::getActiveWindow()->getCurrentHeight())));
  GL::Renderer::drawElements(GL::RenderMode::TRIANGLES, 6 * text.size());
}
