#ifndef _FINGERPRINT_CONNECT_FONT_ATLAS_HPP
#define _FINGERPRINT_CONNECT_FONT_ATLAS_HPP

#include "src/font.hpp"
#include "wrapgl/texture.hpp"
#include <array>

class FontAtlas {
private:
  GL::Texture2D m_Texture;
  std::array<glm::vec2, 96>
      m_EndTC; // It has one extra element, which just helps it remove some ifs.

  std::array<glm::vec2, 95> m_Advance;
  std::array<glm::uvec2, 95> m_Dimension;
  std::array<glm::uvec2, 95> m_Bearing;

private:
  FontAtlas(const glm::uvec2 atlasDimension, const Font &font);

public:
  static FontAtlas create(const Font &font, const unsigned int pixelSize = 48);

public:
  glm::vec4 getTexCoordRectForChar(const char c) const;
  glm::vec2 getAdvanceForChar(const char c) const;
  glm::uvec2 getDimensionForChar(const char c) const;
  glm::vec2 getBearingForChar(const char c) const;
  void bindTextureAt(const unsigned int idx) const;
};

#endif
