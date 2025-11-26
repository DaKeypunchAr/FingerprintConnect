#ifndef _FINGERPRINT_CONNECT_FONT_HPP
#define _FINGERPRINT_CONNECT_FONT_HPP

#include <filesystem>
#include <ft2build.h>
#include <print>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

class GlyphInfo {
private:
  void *m_RenderedData;
  glm::uvec2 m_GlyphDimension;
  glm::vec2 m_Advance;
  glm::vec2 m_Bearing;

public:
  GlyphInfo(const glm::uvec2 dimension, const glm::vec2 advance,
            const glm::vec2 bearing, void *const data = nullptr)
      : m_RenderedData(data), m_GlyphDimension(dimension), m_Advance(advance),
        m_Bearing(bearing) {}

public:
  glm::uvec2 getDimension() const { return m_GlyphDimension; }
  glm::vec2 getAdvance() const { return m_Advance; }
  glm::vec2 getBearing() const { return m_Bearing; }

  bool hasRenderedData() const { return m_RenderedData != nullptr; }
  void *getRenderedDataPtr() const {
    if (!hasRenderedData())
      std::println(stderr, "No rendered data in the glyph");

    return m_RenderedData;
  }
};

struct FTLibWrapper {
private:
  // FT_Library is secretly a pointer..
  FT_Library m_FTLib;

public:
  FTLibWrapper() : m_FTLib(nullptr) {}
  ~FTLibWrapper() {
    if (isInitialized())
      FT_Done_FreeType(m_FTLib);
  }
  inline bool isInitialized() const { return m_FTLib != nullptr; }
  inline void initializeFreeType() {
    if (!isInitialized()) {
      FT_Error error = FT_Init_FreeType(&m_FTLib);
      if (error == FT_Err_Ok)
        return;

      throw std::runtime_error("Unable to InitializeFreeType!");
    } else {
      std::print(stderr,
                 "Initializing FreeType when it is already initialized!");
    }
  }
  inline void doneFreeType() {
    if (isInitialized()) {
      FT_Done_FreeType(m_FTLib);
      m_FTLib = nullptr;
    } else {
      std::print(stderr,
                 "Destructing FreeType when it is not even initialized!");
    }
  }
  inline FT_Library getRawPtr() { return m_FTLib; }
};

class Font {
private:
  static FTLibWrapper s_FT;

private:
  FT_Face m_Face;

private:
  Font(const FT_Face face) : m_Face(face) {}

public:
  static void InitializeFreeType();
  static void DoneFreeType();
  static bool IsFreeTypeInitialized() { return s_FT.isInitialized(); }

public:
  static Font create(const std::filesystem::path &filePath);

public:
  void free() const;

  void setPixelSize(const unsigned int pixelSize) const {
    FT_Set_Pixel_Sizes(m_Face, 0, pixelSize);
  }
  GlyphInfo loadCharacter(const char c, const bool render = false) const;
};

#endif
