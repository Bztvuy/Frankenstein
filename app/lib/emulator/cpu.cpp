#include "cpu.h"
#include <circle/util.h>

CPU::CPU(const Rom* rom)
{
	const iNesHeader* header = rom->GetHeader();
	int prgRomBanks = header->prgRomBanks;
	int trainerOffset = rom->GetTrainerOffset();
	int prgRomBanksLocation = rom->headerSize + trainerOffset;
	
	switch (prgRomBanks) {
		case 2:
			memcpy(memory->prgRomUpperBank, rom->GetRaw() + prgRomBanksLocation + prgRomBankSize, prgRomBankSize);
		case 1:
			memcpy(memory->prgRomLowerBank, rom->GetRaw() + prgRomBanksLocation, prgRomBankSize);
			break;
		default: //TODO: implement multiple PRG-ROM banks
            break;
	}
}
