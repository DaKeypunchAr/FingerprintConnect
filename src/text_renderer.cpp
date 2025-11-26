#include "text_renderer.hpp"
#include "src/font_atlas.hpp"
#include "wrapgl/renderer.hpp"
#include "wrapgl/vertex_buffer.hpp"
#include "wrapgl_core.hpp" // IWYU pragma: keep

namespace TextRenderer {

void renderText(const std::string_view text, const glm::vec4 color,
                const glm::vec2 pos, const float scale,
                const GL::ShaderProgram &program, const FontAtlas &atlas,
                const glm::uvec2 windowDimension) {
  // TODO: Update WrapGL to have Index Buffer and let us do more uniform stuff
  // and so that we can give it something like uniform buffer.

  GL::VertexArray character = GL::VertexArray::create();

  GL::VertexBuffer posBuffer = GL::VertexBuffer::createAndAllocate(
      sizeof(float) * 2 * 4, GL::BufferFrequencyHint::DYNAMIC);

  GL::VertexBuffer texBuffer = GL::VertexBuffer::createAndAllocate(
      sizeof(float) * 2 * 4, GL::BufferFrequencyHint::DYNAMIC);

  character.setBufferBindings(
      {GL::Binding::create(posBuffer, 0, 0, sizeof(float) * 2),
       GL::Binding::create(texBuffer, 1, 0, sizeof(float) * 2)});

  character.setFormat({GL::Attribute::create(GL::DataType::F32, 2, 0),
                       GL::Attribute::create(GL::DataType::F32, 2, 1)});

  character.select();
  program.select();
  program.setUniform("tex", 0);

  glm::vec2 pen = pos;
  for (char c : text) {
    glm::vec2 dim = glm::vec2(atlas.getDimensionForChar(c)) * scale;
    glm::vec2 bearing = atlas.getBearingForChar(c) * scale;

    glm::vec4 bltr = glm::vec4(pen.x + bearing.x, pen.y + bearing.y - dim.y,
                               pen.x + bearing.x + dim.x, pen.y + bearing.y);

    bltr.x = (bltr.x / windowDimension.x) * 2.0f - 1.0f;
    bltr.z = (bltr.z / windowDimension.x) * 2.0f - 1.0f;
    bltr.y = (bltr.y / windowDimension.y) * 2.0f - 1.0f;
    bltr.w = (bltr.w / windowDimension.y) * 2.0f - 1.0f;

    posBuffer.update(
        {bltr.x, bltr.y, bltr.z, bltr.y, bltr.x, bltr.w, bltr.z, bltr.w});

    glm::vec4 texCoord = atlas.getTexCoordRectForChar(c);
    texBuffer.update({texCoord.x, texCoord.y, texCoord.z, texCoord.y,
                      texCoord.x, texCoord.w, texCoord.z, texCoord.w});

    GL::Renderer::drawArrays(character, program, GL::RenderMode::TRIANGLE_STRIP,
                             0, 4);

    glm::vec2 advance = atlas.getAdvanceForChar(c) * scale;
    pen += advance;
  }
}

} // namespace TextRenderer
