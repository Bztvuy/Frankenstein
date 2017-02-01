#ifndef NES_H
#define NES_H

class Nes
{
private:
    Rom* rom;
	Cpu* cpu;
    Ppu* ppu;
public:
	Nes();
};

#endif // CPU_H
