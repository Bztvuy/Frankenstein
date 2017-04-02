project('frankenstein', 'cpp',
  version: '0.1.0',
  meson_version: '>= 0.36.0',
  default_options: ['buildtype=debugoptimized',
                    'cpp_std=c++11']
)

thread = dependency('threads', native: true)
gtest = subproject('gtest', native: true, main: false)
gtest_inc = gtest.get_variable('incdir')
gtest_lib_src = gtest.get_variable('libsources')
gtest_main_src = gtest.get_variable('mainsources')

#-------------------------------------------------------------------------------
# compilation variables
#-------------------------------------------------------------------------------
cpp_args = ['-Wall', '-Wno-psabi', '-fsigned-char']

if meson.is_cross_build()
    rpi_version = meson.get_cross_property('rpi_version')
    rpi_version_arg = ['-DRASPPI=@0@'.format(rpi_version)]

    cpp_args += rpi_version_arg
    cpp_cross_args = cpp_args + ['-fno-builtin', '-nostdinc', '-nostdlib', '-ffreestanding', 
                                 '-D__circle__', '-fno-exceptions', '-fno-non-call-exceptions', 
                                 '-fno-rtti']
endif

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
if not meson.is_cross_build()
    subdir('emulator')
else
    circle = subproject('circle')

    libcircle_dep = circle.get_variable('libcircle_dep')
    libcircle_include = circle.get_variable('circle_include')
    
    subdir('emulator')
    subdir('application')
    subdir('kernel')
endif
