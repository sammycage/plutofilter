project('plutofilter', 'c',
  version: '1.0',
  meson_version: '>=1.0.0',
  license: 'MIT',
)

cc = meson.get_compiler('c')

math_dep = cc.find_library('m')

plutofilter_dep = declare_dependency(
  include_directories: include_directories('.'),
  dependencies: math_dep,
)

if not meson.is_subproject()
  plutofilter_source = configure_file(
    input: 'plutofilter.h',
    output: 'plutofilter.c',
    copy: true,
  )

  plutofilter_lib = library('plutofilter', plutofilter_source,
    c_args: ['-DPLUTOFILTER_IMPLEMENTATION'],
    dependencies: plutofilter_dep,
    install: true,
  )

  install_headers('plutofilter.h', subdir: 'plutofilter')

  pkgmod = import('pkgconfig')
  pkgmod.generate(plutofilter_lib,
    name: 'PlutoFilter',
    description: 'Image filter library',
    url: 'https://github.com/sammycage/plutofilter/',
    filebase: 'plutofilter',
    subdirs: 'plutofilter'
  )

  subdir('examples')
endif
