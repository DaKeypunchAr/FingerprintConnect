#ifndef FINGERPRINT_CONNECT_LAYOUT_HPP
#define FINGERPRINT_CONNECT_LAYOUT_HPP

#include "src/font_atlas.hpp"
#include "wrapgl/shader_program.hpp"
#include "wrapgl/vertex_array.hpp"
#include "wrapgl_core.hpp" // IWYU pragma: keep
#include <filesystem>

class Layout {
private:
  GL::Texture2D m_Reference;
  float m_RefScale;
  glm::vec2 m_RefPos;

  GL::VertexArray m_RefVA;
  GL::VertexArray m_PointVA;
  GL::VertexBuffer m_RefPosVB;
  GL::VertexBuffer m_RefTexVB;
  GL::ShaderProgram m_TexShader;
  GL::ShaderProgram m_PointShader;

  bool m_ShowKeys = false;
  std::array<glm::vec2, 26> m_OffsetOfKeys;
  bool m_OffsetEditMode = false;
  bool m_ShowReference = false;
  unsigned char m_EditModeSelectedChar = 0;

  std::filesystem::path m_ConfigPath;
  std::filesystem::path m_TexturePath;

  struct Config {
    const std::filesystem::path texPath;
    const std::array<glm::vec2, 26> offsets;
    const std::filesystem::path path;
  };
  static Config loadConfig(const std::filesystem::path &configPath);

  Layout(const std::filesystem::path &texturePath,
         const std::array<glm::vec2, 26> &offsetOfKeys,
         const std::filesystem::path &path);
  Layout(const Config &config);

  void saveConfig() const;

public:
  Layout(const std::filesystem::path &configPath);

  void render(const FontAtlas &atlas, const GL::ShaderProgram &textProgram);

  inline float getScale() const { return m_RefScale; }
  inline void setScale(const float scale) { m_RefScale = scale; }

  inline glm::vec2 getPos() const { return m_RefPos; }
  inline void setPos(const glm::vec2 pos) { m_RefPos = pos; }

  inline bool getShowKeys() const { return m_ShowKeys; }
  inline void setShowKeys(const bool showKeys) { m_ShowKeys = showKeys; }

  inline bool getOffsetEditMode() const { return m_OffsetEditMode; }
  inline void setOffsetEditMode(const bool offEditMode) {
    m_OffsetEditMode = offEditMode;
  }

  inline bool getShowReference() const { return m_ShowReference; }
  inline void setShowReference(const bool showReference) {
    m_ShowReference = showReference;
  }

  inline unsigned char getEditModeSelectedChar() const {
    return m_EditModeSelectedChar;
  }
  inline void setEditModeSelectedChar(const unsigned char idx) {
    m_EditModeSelectedChar = idx;
  }

  inline glm::vec2 getOffset(const unsigned char idx) {
    return m_OffsetOfKeys[idx];
  }

  void update();
  void updateBuffer() const;
};

#endif
