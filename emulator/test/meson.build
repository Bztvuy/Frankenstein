#gtest = subproject('gtest', native: true)

emuTests = executable('emulator_tests', 'tests-main.cpp', 'cputest.cpp',
	link_with: emulator_native,
	include_directories: emulator_include,
        cpp_args: cpp_args,
	native: true)
#        dependencies: gtest)

test('can_run_tests', emuTests)
