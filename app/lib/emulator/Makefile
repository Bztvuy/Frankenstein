#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= rom.o cpu.o ppu.o

libemu.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
