#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= rom.o cpu.o ppu.o nes.o

libemulator.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
