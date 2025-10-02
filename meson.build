project(
  'fingerprint-connect',
  'cpp',
  default_options: ['cpp_std=c++23']
)

wrapgl_dep = dependency('wrapgl', required: true)

sources = ['main.cpp',] 

executable(
  'main',
  sources,
  dependencies: [wrapgl_dep],
  include_directories: include_directories('include'),
)
