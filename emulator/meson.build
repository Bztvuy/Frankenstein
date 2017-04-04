emulator_src = ['memory_nes.cpp', 'rom.cpp', 'cpu.cpp', 'ppu.cpp', 'nes.cpp',
                'gamepad.cpp', 'rom_static_data.cpp', 'mapper_factory.cpp', 'mapper.cpp']

emulator_include = include_directories('include')

if compiler.get_id() == 'clang'
    emulator_native = static_library('emulator_native', emulator_src, 'rom_loader.cpp',
        include_directories: emulator_include,
        cpp_args: cpp_args + ['-Weverything', '-Wno-c++98-compat', '-Wno-c++98-compat-pedantic', '-Wno-c++14-binary-literal', '-Wno-padded'],
        native: true)
else
    emulator_native = static_library('emulator_native', emulator_src, 'rom_loader.cpp',
        include_directories: emulator_include,
            cpp_args: cpp_args,
        native: true)
endif

subdir('test')

if meson.is_cross_build()
    emulator = static_library('emulator', emulator_src,
                    include_directories: [emulator_include, libcircle_include],
                    cpp_args: ['-DNotNative'] + cpp_cross_args)

    emulator_dep = declare_dependency(
            link_with: emulator,
            include_directories: emulator_include)
endif
