#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= rom.o cpu.o ppu.o nes.o cpu_addressing.o

libemu.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
