emulator_src = ['memory.cpp', 'rom.cpp', 'cpu.cpp', 'ppu.cpp', 'nes.cpp',
                'gamepad.cpp', 'rom_static.cpp', 'rom_static_data.cpp']
emulator_include = include_directories('include')

emulator_native = static_library('emulator_native', emulator_src, 'rom_file.cpp',
    include_directories: emulator_include,
        cpp_args: cpp_args,
    native: true)

subdir('test')

if meson.is_cross_build()
    emulator = static_library('emulator', emulator_src,
                    include_directories: [emulator_include, libcircle_include],
                    cpp_args: ['-DNotNative'] + cpp_cross_args)

    emulator_dep = declare_dependency(
            link_with: emulator,
            include_directories: emulator_include)
endif
