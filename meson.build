project(
    'plutofilter',
    'c',
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

    # Meson will not compile a file with *.h
    fixed_ext = configure_file(
        input: 'plutofilter.h',
        output: 'plutofilter.c',
        copy: true,
    )

    plutolib = library(
        'plutofilter',
        fixed_ext,
        c_args: ['-DPLUTOFILTER_IMPLEMENTATION'],
        dependencies: plutofilter_dep,
        install: true,
    )

    install_headers('plutofilter.h', subdir: 'plutofilter')
    
    pkg = import('pkgconfig')
    pkg.generate(
        plutolib,
        url: 'https://github.com/sammycage/plutofilter/',
        description: 'single-header, zero-allocation image filter library',
    )

    subdir('examples')

endif
