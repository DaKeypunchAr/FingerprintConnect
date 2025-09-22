project('glad', 'c')

glad_lib = library(
  'glad',
  'src/glad.c',
  include_directories: include_directories('include'),
  install: false,
)

glad_dep = declare_dependency(include_directories: include_directories('include'), link_with: glad_lib)

meson.override_dependency('glad', glad_dep)
