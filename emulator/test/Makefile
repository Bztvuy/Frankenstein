CIRCLEHOME = ../../..
EMULATOR = ../emulator

CXX=g++
CXXFLAGS=-std=c++14

all: libemulator.a cputest
	
OBJS	= $(EMULATOR)/rom.o $(EMULATOR)/cpu.o $(EMULATOR)/ppu.o $(EMULATOR)/nes.o

libemulator.a: $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)

cputest: cputest.cpp
	$(CXX) $(CXXFLAGS) tests-main.o cputest.cpp -o cputest -I$(CIRCLEHOME)/app/lib/emulator -L. -lemulator