#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= rom.o cpu.o memory.o ppu.o

libtempl.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
