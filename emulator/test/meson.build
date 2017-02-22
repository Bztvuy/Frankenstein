emuTests = executable('emulator_tests', 'tests-main.cpp', 'cputest.cpp',
	link_with: emulator_native,
	include_directories: emulator_include,
	native: true)

test('can_run_tests', emuTests)
