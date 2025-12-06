project('fingerprint-connect', 'cpp', default_options: ['cpp_std=c++23'])

wrapgl_dep = dependency('wrapgl', required: true)
freetype_dep = dependency('freetype2', required: true)
imgui_dep = dependency('imgui', required: true)

sources = [
  'main.cpp',
  'src/font.cpp',
  'src/font_atlas.cpp',
  'src/text_renderer.cpp',
  'src/layout.cpp',
]

executable(
  'main',
  sources,
  dependencies: [wrapgl_dep, freetype_dep, imgui_dep],
)
