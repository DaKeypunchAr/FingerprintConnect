#ifndef FINGERPRINT_CONNECT_TEXT_RENDERER_HPP
#define FINGERPRINT_CONNECT_TEXT_RENDERER_HPP

#include "glm/glm.hpp" // IWYU pragma: keep
#include "src/font_atlas.hpp"
#include "wrapgl/shader_program.hpp"
#include <string_view>

namespace TextRenderer {
void renderText(const std::string_view text, const glm::vec4 color,
                const glm::vec2 pos, const float scale,
                const GL::ShaderProgram &program, const FontAtlas &atlas,
                const glm::uvec2 windowDimension);
}

#endif
