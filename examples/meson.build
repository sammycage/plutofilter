fs = import('fs')

fs.copyfile('zhang-hanyun.jpg')
fs.copyfile('royal-purple.png')
fs.copyfile('firebrick-circle.png')

example_dep = declare_dependency(
  sources: ['example.c'],
  dependencies: plutofilter_dep
)

zhang_hanyun_path = meson.current_build_dir() / 'zhang-hanyun.jpg'
royal_purple_path = meson.current_build_dir() / 'royal-purple.png'
firebrick_circle_path = meson.current_build_dir() / 'firebrick-circle.png'

blend_modes = [
  'normal',
  'multiply',
  'screen',
  'overlay',
  'darken',
  'lighten',
  'color-dodge',
  'color-burn',
  'hard-light',
  'soft-light',
  'difference',
  'exclusion'
]

blend_tests = {}
foreach mode : blend_modes
  blend_tests += {'zhang-hanyun-royal-purple-' + mode: [zhang_hanyun_path, royal_purple_path, mode]}
endforeach

composite_operators = [
  'over',
  'in',
  'out',
  'atop',
  'xor'
]

composite_tests = {}
foreach op : composite_operators
  composite_tests += {'zhang-hanyun-firebrick-circle-' + op: [zhang_hanyun_path, firebrick_circle_path, op]}
endforeach

blur_radii = [
  ['0', '0'],
  ['5', '5'],
  ['10', '10']
]

blur_tests = {}
foreach radii : blur_radii
  blur_tests += {'zhang-hanyun-blur-' + '-'.join(radii): [zhang_hanyun_path] + radii}
endforeach

grayscale_tests = {}
foreach amount : ['0', '0.25', '0.5', '0.75', '1']
  grayscale_tests += {'zhang-hanyun-grayscale-' + amount: [zhang_hanyun_path, amount]}
endforeach

sepia_tests = {}
foreach amount : ['0', '0.25', '0.5', '0.75', '1']
  sepia_tests += {'zhang-hanyun-sepia-' + amount: [zhang_hanyun_path, amount]}
endforeach

saturate_tests = {}
foreach amount : ['1', '4', '0.5', '0']
  saturate_tests += {'zhang-hanyun-saturate-' + amount: [zhang_hanyun_path, amount]}
endforeach

contrast_tests = {}
foreach amount : ['1', '1.75', '0.5', '0']
  contrast_tests += {'zhang-hanyun-contrast-' + amount: [zhang_hanyun_path, amount]}
endforeach

brightness_tests = {}
foreach amount : ['1', '1.75', '0.5', '0']
  brightness_tests += {'zhang-hanyun-brightness-' + amount: [zhang_hanyun_path, amount]}
endforeach

opacity_tests = {}
foreach amount : ['0', '0.25', '0.5', '0.75', '1']
  opacity_tests += {'zhang-hanyun-opacity-' + amount: [zhang_hanyun_path, amount]}
endforeach

invert_tests = {}
foreach amount : ['0', '0.25', '0.5', '0.75', '1']
  invert_tests += {'zhang-hanyun-invert-' + amount: [zhang_hanyun_path, amount]}
endforeach

hue_rotate_tests = {}
foreach amount : ['0', '30', '90', '180', '270', '360']
  hue_rotate_tests += {'zhang-hanyun-hue-rotate-' + amount: [zhang_hanyun_path, amount]}
endforeach

arithmetic_coefficients = [
  ['0', '1', '1', '0'],
  ['0.5', '0.5', '0.5', '0'],
  ['1', '0', '0', '0'],
  ['1', '0', '0', '0.5'],
]

arithmetic_tests = {}
foreach coeff : arithmetic_coefficients
  arithmetic_tests += {'zhang-hanyun-firebrick-circle-arithmetic-' + '-'.join(coeff): [zhang_hanyun_path, firebrick_circle_path] + coeff}
endforeach

example_sources = {
  'blend.c': blend_tests,
  'composite.c': composite_tests,
  'blur.c': blur_tests,
  'grayscale.c': grayscale_tests,
  'sepia.c': sepia_tests,
  'saturate.c': saturate_tests,
  'contrast.c': contrast_tests,
  'brightness.c': brightness_tests,
  'opacity.c': opacity_tests,
  'invert.c': invert_tests,
  'hue-rotate.c': hue_rotate_tests,
  'arithmetic.c': arithmetic_tests,
}

foreach source_file, test_cases : example_sources
  exe_name = source_file.replace('.c', '')
  exe = executable(exe_name, source_file,
    dependencies: example_dep,
    build_by_default: false
  )

  foreach test_name, args : test_cases
    test(test_name, exe, args: args)
  endforeach
endforeach
