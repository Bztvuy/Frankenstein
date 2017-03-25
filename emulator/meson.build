emulator_src = ['memory.cpp', 'rom.cpp', 'cpu.cpp', 'ppu.cpp', 'nes.cpp', 'gamepad.cpp']
emulator_include = include_directories('include')

<<<<<<< HEAD
emulator_native = static_library('emulator_native', emulator_src, 'rom_file.cpp',
    include_directories: emulator_include,
=======
emulator_native = static_library('emulator_native', [emulator_src, 'file_rom.cpp'],
	include_directories: emulator_include,
>>>>>>> 72ec4108796e94e0a2c277c4d45982e3a393de20
        cpp_args: cpp_args,
    native: true)

subdir('test')

if meson.is_cross_build()
    emulator = static_library('emulator', emulator_src, 'rom_static.cpp', 'rom_static_data.cpp',
                    include_directories: [emulator_include, libcircle_include],
                    cpp_args: ['-DNotNative'] + cpp_cross_args)

    emulator_dep = declare_dependency(
            link_with: emulator,
            include_directories: emulator_include)
endif
