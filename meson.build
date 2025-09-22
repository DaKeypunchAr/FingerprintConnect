project(
  'fingerprint-connect',
  'cpp',
  default_options: ['cpp_std=c++23', '-stdlib=libc++'],
)

libcxx_flags = [
  '-nostdinc++',
  '-I/usr/include/c++/v1',
  '-isystem', '/usr/include',
  '-isystem', '/usr/include/x86_64-linux-gnu',
]
add_project_arguments(libcxx_flags, language: 'cpp')
add_project_link_arguments(['-lc++', '-lc++abi'], language: 'cpp')

glfw_dep = dependency('glfw3', fallback: ['glfw', 'glfw_dep'])
fmt_dep = dependency('fmt')
glad = dependency('glad')
glm = dependency('glm')
executable('main', 'main.cpp', dependencies: [glfw_dep, fmt_dep, glad, glm])
