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

cpp_args       = rpi_version_arg + ['-Wall', '-Wno-psabi', '-fsigned-char']
cpp_cross_args = cpp_args + ['-fno-builtin', '-nostdinc', '-nostdlib', '-ffreestanding', 
                             '-D__circle__', '-fno-exceptions', '-fno-non-call-exceptions', 
                             '-fno-rtti']

add_project_arguments(cpp_cross_args, language:'cpp')

#-------------------------------------------------------------------------------
# targets
#-------------------------------------------------------------------------------
circle_include = include_directories('include')
ld_script =  files('circle.ld')
ld_script_location = '@0@/circle.ld'.format(meson.current_source_dir())

subdir('boot')
subdir('lib')
