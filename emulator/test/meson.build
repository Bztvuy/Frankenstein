thread = dependency('threads', native: true)
gtest_dep = library('gtest', gtest_lib_src, gtest_main_src,
    include_directories: gtest_inc,
    dependencies: thread,
    native: true)

sfml = [dependency('sfml-graphics', native: true),
        dependency('sfml-window', native: true)]

sfmlEmulator = executable('sfml_emulator', 'sfmlEmulator.cpp',
    link_with: [emulator_native],
    dependencies: sfml,
    include_directories: [emulator_include],
    cpp_args: cpp_args,
    native: true)

emuTests = executable('emulator_tests', 'cpu_test.cpp', 'memory_test.cpp',
    link_with: [emulator_native, gtest_dep],
    include_directories: [emulator_include, gtest_inc],
    cpp_args: cpp_args,
    native: true)

test('can_run_tests', emuTests, native: true)
