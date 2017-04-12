if not meson.is_cross_build() or host_machine.system() != 'raspberry-pi'
    error('The kernel must target a valid RaspberryPi target')
endif

elf = executable('kernel.elf', 'startup.S', 'main.cpp', 'kernel.cpp',
    dependencies: [libcircle_dep, emulator_dep],
    cpp_args: cpp_cross_args,
    link_args: cpp_cross_args)

objcopy_name = meson.get_cross_property('objcopy')
objcopy = find_program(objcopy_name)
target = meson.get_cross_property('compilation_target')

custom_target('kernel7.img', 
    input: elf,
    output: '@0@.img'.format(target),
    command: [objcopy, '@INPUT@', '-O', 'binary', '@OUTPUT@'],
    install: true,
    install_dir: 'kernel')
