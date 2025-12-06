#include "src/font.hpp"
#include "wrapgl/texture.hpp"
#include <print>
#include <src/font_atlas.hpp>

FontAtlas FontAtlas::create(const Font &font, const unsigned int pixelSize) {
  font.setPixelSize(pixelSize);
  glm::uvec2 atlasDimension(0);

  for (char c = 32; c < 127; c++) {
    // TODO: you are supposed to optimize the bottom line.
    GlyphInfo info = font.loadCharacter(c);

    glm::uvec2 glyphDimension = info.getDimension();
    atlasDimension.x += glyphDimension.x;
    atlasDimension.y =
        (glyphDimension.y > atlasDimension.y) * glyphDimension.y +
        (glyphDimension.y <= atlasDimension.y) * atlasDimension.y;
  }
  return FontAtlas(atlasDimension, font);
}

FontAtlas::FontAtlas(const glm::uvec2 atlasDimension, const Font &font)
    : m_Texture(GL::Texture2D::create(atlasDimension, GL::TextureFormat::R8)),
      m_EndTC() {
  unsigned int x = 0;
  unsigned int i = 0;
  for (char c = 32; c < 127; c++, i++) {
    GlyphInfo info = font.loadCharacter(c, true);
    if (!info.hasRenderedData()) {
      if (c != 32) {
        std::println(stderr,
                     "Character '{0}' or {1} doesn't have any rendered data!",
                     c, (int)c);
      } else {
        m_Advance[i] = info.getAdvance();
        m_Bearing[i] = info.getBearing();
        m_Dimension[i] = glm::vec2(0);
      }
      continue;
    }

    m_Texture.update(info.getRenderedDataPtr(), GL::TextureFormat::R8,
                     glm::uvec2(x, 0), info.getDimension());

    x += info.getDimension().x;

    m_Dimension[i] = info.getDimension();

    glm::vec2 endTC =
        glm::vec2(x / (float)atlasDimension.x,
                  info.getDimension().y / (float)atlasDimension.y);
    m_EndTC[i + 1] = endTC;

    m_Advance[i] = info.getAdvance();
    m_Bearing[i] = info.getBearing();
  }
  m_EndTC[0] = glm::vec2(0.0f, m_EndTC[1].y);

  m_Texture.bind(0);
}

glm::vec4 FontAtlas::getTexCoordRectForChar(const char ch) const {
  if (ch < 32 || ch == 127) {
    std::println(stderr, "Character not in the atlas!");
  }

  unsigned char c = ch - 32;
  return glm::vec4(m_EndTC[c].x, m_EndTC[c + 1].y, m_EndTC[c + 1].x, 0.0f);
}

glm::vec2 FontAtlas::getAdvanceForChar(const char ch) const {
  if (ch < 32 || ch == 127) {
    std::println(stderr, "Character not in the atlas!");
  }

  unsigned char c = ch - 32;
  return m_Advance[c];
}

glm::uvec2 FontAtlas::getDimensionForChar(const char ch) const {
  if (ch < 32 || ch == 127) {
    std::println(stderr, "Character not in the atlas!");
  }

  unsigned char c = ch - 32;
  return m_Dimension[c];
}

glm::vec2 FontAtlas::getBearingForChar(const char ch) const {
  if (ch < 32 || ch == 127) {
    std::println(stderr, "Character not in the atlas!");
  }

  unsigned char c = ch - 32;
  return m_Bearing[c];
}

void FontAtlas::bindTextureAt(const unsigned int idx) const {
  m_Texture.bind(idx);
}
