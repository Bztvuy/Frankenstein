project('circle', 'cpp',
    version: '1.0.0',
    license: 'GPLv3')

if not meson.is_cross_build() or host_machine.system() != 'raspberry-pi'
    error('circle library must target a valid RaspberryPi target')
endif

#-------------------------------------------------------------------------------
# compilation variables
#-------------------------------------------------------------------------------
rpi_version = meson.get_cross_property('rpi_version')
rpi_version_arg = ['-DRASPPI=@0@'.format(rpi_version)]
optimisation = 2

a_args = meson.get_cross_property('a_args')
c_args = meson.get_cross_property('c_args')

a_source_args = a_args + rpi_version_arg
c_source_args = c_args + rpi_version_arg + ['-Wall', '-Wno-psabi', '-fsigned-char', '-fno-builtin',
                 '-nostdinc', '-nostdlib', '-D__circle__']
cpp_source_args = c_source_args + ['-fno-exceptions', '-fno-rtti', '-std=c++0x']

add_project_arguments(a_source_args, language:'assembly')
add_project_arguments(c_source_args, language:'c')
add_project_arguments(cpp_source_args, language:'cpp')
add_project_link_arguments(c_source_args, language:'c')
add_project_link_arguments(cpp_source_args, language:'cpp')

#-------------------------------------------------------------------------------
# targets
#-------------------------------------------------------------------------------
circle_include = include_directories('include')
ld_script =  files('circle.ld')
ld_script_location = '@0@/circle.ld'.format(meson.current_source_dir())

#subdir('boot')
subdir('lib')
