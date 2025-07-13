project('plutofilter', 'c', version: '1.0')

cc = meson.get_compiler('c')

math_dep = cc.find_library('m')

plutofilter_dep = declare_dependency(
  include_directories: include_directories('.'),
  dependencies: math_dep
)

subdir('examples')
