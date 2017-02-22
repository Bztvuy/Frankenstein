subdir('sched')
subdir('usb')

circle_src = ['actled.cpp', 'alloc.cpp', 'bcmmailbox.cpp', 'bcmpropertytags.cpp',
              'gpiopin.cpp', 'libhelper.cpp', 'memory.cpp', 'new.cpp', 'purecall.cpp',
              'synchronize.cpp', 'sysinit.cpp', 'timer.cpp', 'util.cpp',
              'bcmframebuffer.cpp', 'device.cpp', 'koptions.cpp', 'pagetable.cpp',
              'screen.cpp', 'nulldevice.cpp', 'assert.cpp',  'chargenerator.cpp',
              'debug.cpp', 'devicenameservice.cpp', 'logger.cpp', 'serial.cpp',
              'string.cpp', 'exceptionhandler.cpp',  'interrupt.cpp', 'gpiomanager.cpp',
              'gpioclock.cpp', 'dmachannel.cpp',  'pwmsounddevice.cpp', 'pwmoutput.cpp',
              'tracer.cpp', 'i2cmaster.cpp', 'i2cslave.cpp',  'spimaster.cpp',
              'spimasterdma.cpp', 'virtualgpiopin.cpp', 'machineinfo.cpp', 'multicore.cpp',
              'spinlock.cpp', 'bcmrandom.cpp', 'cputhrottle.cpp', 'ptrarray.cpp',
              'ptrlist.cpp', 'time.cpp', 'cache-v7.S', 'libstub.S', 'util_fast.S',
              'delayloop.S', 'exceptionstub.S']

libcircle = static_library('circle', circle_src, include_directories: circle_include)

libcircle_dep = declare_dependency(
	include_directories: circle_include,
	link_with: libcircle,
	link_depends: ld_script,
	link_args: ['-Wl,-Map,kernel7.map,-T,@0@'.format(ld_script_location), '-fno-builtin','-nostdinc', '-nostdlib'],
	sources: ['startup.S'])
