project('frankenstein', 'cpp',
  version: '0.1.0',
  meson_version: '>= 0.36.0',
)

#-------------------------------------------------------------------------------
# compilation variables
#-------------------------------------------------------------------------------

if meson.is_cross_build()
	rpi_version = meson.get_cross_property('rpi_version')
	rpi_version_arg = ['-DRASPPI=@0@'.format(rpi_version)]

	a_args = meson.get_cross_property('a_args')
	c_args = meson.get_cross_property('c_args')
	a_source_args = a_args + rpi_version_arg
	c_source_args = c_args + rpi_version_arg + ['-Wall', '-Wno-psabi', '-fsigned-char', '-fno-builtin',
		         '-nostdinc', '-nostdlib', '-D__circle__']
	cpp_source_args = c_source_args + ['-fno-exceptions', '-fno-rtti', '-std=c++14']

	#-------------------------------------------------------------------------------
	# Targets
	#-------------------------------------------------------------------------------
	circle = subproject('circle')

	libcircle_dep = circle.get_variable('libcircle_dep')
	libcircle_include = circle.get_variable('circle_include')
	libsched_dep = circle.get_variable('libsched_dep')
	libusb_dep = circle.get_variable('libusb_dep')
endif

subdir('emulator')

if meson.is_cross_build()
	subdir('kernel')
endif
