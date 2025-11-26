#include "font.hpp"
#include "freetype/freetype.h"
#include <stdexcept>

FTLibWrapper Font::s_FT;

void Font::InitializeFreeType() { s_FT.initializeFreeType(); }

void Font::DoneFreeType() { s_FT.doneFreeType(); }

void Font::free() const { FT_Done_Face(m_Face); }

Font Font::create(const std::filesystem::path &filePath) {
  FT_Face face;
  if (!s_FT.isInitialized())
    s_FT.initializeFreeType();

  FT_Error err = FT_New_Face(s_FT.getRawPtr(), filePath.c_str(), 0, &face);

  if (err == FT_Err_Unknown_File_Format) {
    throw std::runtime_error("File format not supported by freetype!");
  } else if (err) {
    throw std::runtime_error(
        "Some form of freetype error occured while creation!");
  }

  return Font(face);
}

GlyphInfo Font::loadCharacter(const char c, const bool render) const {
  FT_Error error = FT_Load_Char(
      m_Face, c, render * FT_LOAD_RENDER + (!render) * FT_LOAD_DEFAULT);

  if (error) {
    std::string err("Failed to load a character from '");
    err += c;
    err += "'";
    throw std::runtime_error(err);
  }

  glm::uvec2 dimension =
      glm::uvec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows);

  glm::vec2 advance =
      glm::vec2(m_Face->glyph->advance.x, m_Face->glyph->advance.y);
  advance /= 64.0f;

  glm::vec2 bearing = glm::vec2(m_Face->glyph->metrics.horiBearingX,
                                m_Face->glyph->metrics.horiBearingY);
  bearing /= 64.0f;

  void *data = render ? m_Face->glyph->bitmap.buffer : nullptr;

  return GlyphInfo(dimension, advance, bearing, data);
}
