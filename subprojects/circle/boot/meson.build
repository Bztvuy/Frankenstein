wget = find_program('wget')

outputs = ['LICENCE.broadcom', 'bootcode.bin', 'fixup.dat', 'start.elf']

foreach file : outputs

    custom_target(file,
	    output: file,
	    command: [wget, '-q', '-O@OUTDIR@/@0@'.format(file), 'https://github.com/raspberrypi/firmware/blob/master/boot/@0@?raw=true'.format(file)],
	    install: true,
	    install_dir: 'kernel'
	)
	
endforeach


