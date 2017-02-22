wget = find_program('wget')

outputs = ['LICENCE.broadcom', 'bootcode.bin', 'fixup.dat', 'start.elf']

foreach file : outputs
    custom_target('download-'+file,
	output: file,
	command: [wget, '-q', '-O@0@'.format(file), 'https://github.com/raspberrypi/firmware/blob/master/boot/@0@?raw=true'.format(file)],
	install_dir: 'boot',
	install: true)
endforeach
