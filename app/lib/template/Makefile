#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= rom.o cpu.o

libtempl.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
