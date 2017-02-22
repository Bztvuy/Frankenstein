if not meson.is_cross_build() or host_machine.system() != 'raspberry-pi'
    error('The kernel must target a valid RaspberryPi target')
endif

elf = executable('kernel.elf', 'main.cpp', 'kernel.cpp',
	dependencies: [emulator_dep, libcircle_dep, libsched_dep, libusb_dep],
	cpp_args: cpp_source_args)

objcopy_name = meson.get_cross_property('objcopy')
objcopy = find_program(objcopy_name)
target = meson.get_cross_property('compilation_target')

run_command(objcopy, '-d', 'kernel.elf', '-O', 'binary', '@0@.img'.format(target))
