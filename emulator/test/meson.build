thread = dependency('threads', native: true)
gtest_dep = library('gtest', gtest_lib_src, gtest_main_src,
    include_directories: gtest_inc,
    dependencies: thread,
    native: true)

emuTests = executable('emulator_tests', 'cpu_test.cpp', 'memory_test.cpp',
    link_with: [emulator_native,gtest_dep],
    include_directories: [emulator_include, gtest_inc],
    cpp_args: cpp_args,
    native: true)

test('can_run_tests', emuTests, native: true)
