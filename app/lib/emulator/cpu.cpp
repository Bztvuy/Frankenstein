#include "cpu.h"
#include <circle/util.h>

Cpu::Cpu(const Rom* rom)
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

unsigned int buildAddress(unsigned char l, unsigned char m)
{
	unsigned int res = m;
	res <<= 8;
	res |= l;
	return res;
}



    void Cpu::BRK(){}
    
    /// Logical OR 
    /// 2 bytes; 6 cycles
    void Cpu::ORA_IND_X(){
        auto regX = this->registers->X;
        PostIndexedIndirect();
    }
    
    void Cpu::ORA_ZP(){}
    void Cpu::ASL_ZP(){}
    void Cpu::PHP(){}
    void Cpu::ORA_IMM(){}
    void Cpu::ASL_ACC(){}
    void Cpu::ORA_ABS(){}
    void Cpu::ASL_ABS(){}
    void Cpu::BPL(){}
    void Cpu::ORA_IND_Y(){}
    void Cpu::ORA_ZP_X(){}
    void Cpu::ASL_ZP_X(){}
    void Cpu::CLC(){}
    void Cpu::ORA_ABS_Y(){}
    void Cpu::ORA_ABS_X(){}
    void Cpu::ASL_ABS_X(){}
    void Cpu::JSR(){}
    void Cpu::AND_IND_X(){}
    void Cpu::BIT_ZP(){}
    void Cpu::AND_ZP(){}
    void Cpu::ROL_ZP(){}
    void Cpu::PLP(){}
    void Cpu::AND_IMM(){}
    void Cpu::ROL_ACC(){}
    void Cpu::BIT_ABS(){}
    void Cpu::AND_ABS(){}
    void Cpu::ROL_ABS(){}
    void Cpu::BMI(){}
    void Cpu::AND_IND_Y(){}
    void Cpu::AND_ZP_X(){}
    void Cpu::ROL_ZP_X(){}
    void Cpu::SEC(){}
    void Cpu::AND_ABS_Y(){}
    void Cpu::AND_ABS_X(){}
    void Cpu::ROL_ABS_X(){}
    void Cpu::RTI(){}
    void Cpu::EOR_IND_X(){}
    void Cpu::EOR_ZP(){}
    void Cpu::LSR_ZP(){}
    void Cpu::PHA(){}
    void Cpu::EOR_IMM(){}
    void Cpu::LSR_ACC(){}
    void Cpu::JMP_ABS(){}
    void Cpu::EOR_ABS(){}
    void Cpu::LSR_ABS(){}
    void Cpu::BVC(){}
    void Cpu::EOR_IND_Y(){}
    void Cpu::EOR_ZP_X(){}
    void Cpu::LSR_ZP_X(){}
    void Cpu::CLI(){}
    void Cpu::EOR_ABS_Y(){}
    void Cpu::EOR_ABS_X(){}
    void Cpu::LSR_ABS_X(){}
    void Cpu::RTS(){}
    void Cpu::ADC_IND_X(){}
    void Cpu::ADC_ZP(){}
    void Cpu::ROR_ZP(){}
    void Cpu::PLA(){}
    void Cpu::ADC_IMM(){}
    void Cpu::ROR_ACC(){}
    void Cpu::JMP_IND(){}
    void Cpu::ADC_ABS(){}
    void Cpu::ROR_ABS(){}
    void Cpu::BVS(){}
    void Cpu::ADC_IND_Y(){}
    void Cpu::ADC_ZP_X(){}
    void Cpu::ROR_ZP_X(){}
    void Cpu::SEI(){}
    void Cpu::ADC_ABS_Y(){}
    void Cpu::ADC_ABS_X(){}
    void Cpu::ROR_ABS_X(){}
    void Cpu::STA_IND_X(){}
    void Cpu::STY_ZP(){}
    void Cpu::STA_ZP(){}
    void Cpu::STX_ZP(){}
    void Cpu::DEY(){}
    void Cpu::TXA(){}
    void Cpu::STY_ABS(){}
    void Cpu::STA_ABS(){}
    void Cpu::STX_ABS(){}
    void Cpu::BCC(){}
    void Cpu::STA_IND_Y(){}
    void Cpu::STY_ZP_X(){}
    void Cpu::STA_ZP_X(){}
    void Cpu::STX_ZP_Y(){}
    void Cpu::TYA(){}
    void Cpu::STA_ABS_Y(){}
    void Cpu::TXS(){}
    void Cpu::STA_ABS_X(){}
    void Cpu::LDY_IMM(){}
    void Cpu::LDA_IND_X(){}
    void Cpu::LDX_IMM(){}
    void Cpu::LDY_ZP(){}
    void Cpu::LDA_ZP(){}
    void Cpu::LDX_ZP(){}
    void Cpu::TAY(){}
    void Cpu::LDA_IMM(){}
    void Cpu::TAX(){}
    void Cpu::LDY_ABS(){}
    void Cpu::LDA_ABS(){}
    void Cpu::LDX_ABS(){}
    void Cpu::BCS(){}
    void Cpu::LDA_IND_Y(){}
    void Cpu::LDY_ZP_X(){}
    void Cpu::LDA_ZP_X(){}
    void Cpu::LDX_ZP_Y(){}
    void Cpu::CLV(){}
    void Cpu::LDA_ABS_Y(){}
    void Cpu::TSX(){}
    void Cpu::LDY_ABS_X(){}
    void Cpu::LDA_ABS_X(){}
    void Cpu::LDX_ABS_Y(){}
    void Cpu::CPY_IMM(){}
    void Cpu::CMP_IND_X(){}
    void Cpu::CPY_ZP(){}
    void Cpu::CMP_ZP(){}
    void Cpu::DEC_ZP(){}
    void Cpu::INY(){}
    void Cpu::CMP_IMM(){}
    void Cpu::DEX(){}
    void Cpu::CPY_ABS(){}
    void Cpu::CMP_ABS(){}
    void Cpu::DEC_ABS(){}
    void Cpu::BNE(){}
    void Cpu::CMP_IND_Y(){}
    void Cpu::CMP_ZP_X(){}
    void Cpu::DEC_ZP_X(){}
    void Cpu::CLD(){}
    void Cpu::CMP_ABS_Y(){}
    void Cpu::CMP_ABS_X(){}
    void Cpu::DEC_ABS_X(){}
    void Cpu::CPX_IMM(){}
    void Cpu::SBC_IND_X(){}
    void Cpu::CPX_ZP(){}
    void Cpu::SBC_ZP(){}
    void Cpu::INC_ZP(){}
    void Cpu::INX(){}
    void Cpu::SBC_IMM(){}
    void Cpu::NOP(){}
    void Cpu::CPX_ABS(){}
    void Cpu::SBC_ABS(){}
    void Cpu::INC_ABS(){}
    void Cpu::BEQ(){}
    void Cpu::SBC_IND_Y(){}
    void Cpu::SBC_ZP_X(){}
    void Cpu::INC_ZP_X(){}
    void Cpu::SED(){}
    void Cpu::SBC_ABS_Y(){}
    void Cpu::SBC_ABS_X(){}
    void Cpu::INC_ABS_X(){}
    void Cpu::UNIMP(){}

int IMM_m(NesCPU& cpu, NesMMU& mmu)
{
	cpu.setOperand(mmu.read(cpu.getPC() + 1));
	return 1;

}

int ABS_m(NesCPU& cpu, NesMMU& mmu)
{

	unsigned char lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = mmu.read(cpu.getPC() + 2);

	cpu.setAddress(buildAddress(lsb, msb));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}

	return 3;
}

int ABX_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned int lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = mmu.read(cpu.getPC() + 2);

	lsb += cpu.getX();
	msb += lsb >> 8;
	cpu.setAddress(buildAddress(lsb, msb));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}

	return 3 + (lsb >> 8);
}

int ABY_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned int lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = mmu.read(cpu.getPC() + 2);

	lsb += cpu.getY();
	msb += lsb >> 8;
	cpu.setAddress(buildAddress(lsb, msb));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}

	return 3 + (lsb >> 8);
}

//Only for the JMP instruction
int IND_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned int lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = mmu.read(cpu.getPC() + 2);

	unsigned int adr = buildAddress(lsb, msb);

	lsb = mmu.read(adr);
	msb = mmu.read(adr + 1);

	cpu.setAddress(buildAddress(lsb, msb));

	lsb = mmu.read(cpu.getAddress());
	msb = mmu.read(cpu.getAddress() + 1);

	adr = buildAddress(lsb, msb);
	cpu.setOperand(adr);

	return 4;
}

int INX_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned char lsb = mmu.read(cpu.getPC() + 1);

	lsb += cpu.getX();

	cpu.setAddress(lsb);
	cpu.setAddress(mmu.read(cpu.getAddress()));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}
	return 5;
}

int INY_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned int lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = mmu.read(cpu.getPC() + 2);

	unsigned int adr = buildAddress(lsb, msb);

	lsb = mmu.read(adr);
	msb = mmu.read(adr + 1);

	lsb += cpu.getY();
	msb += lsb >> 8;
	cpu.setAddress(buildAddress(lsb, msb));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}
	return 4 + (lsb >> 8);
}

int ZPA_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned char lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = 0x00;
	lsb += cpu.getX();

	cpu.setAddress(buildAddress(lsb, msb));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}
	return 2;
}

int ZPX_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned int lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = 0x00;

	lsb += cpu.getX();
	msb += lsb >> 8;

	cpu.setAddress(buildAddress(lsb, msb));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}
	return 3;
}

int ZPY_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned int lsb = mmu.read(cpu.getPC() + 1);
	unsigned char msb = 0x00;

	lsb += cpu.getY();
	msb += lsb >> 8;

	cpu.setAddress(buildAddress(lsb, msb));

	if (!cpu.getStoreFlag())
	{
		cpu.setOperand(mmu.read(cpu.getAddress()));
	}
	return 3;
}

int REL_m(NesCPU& cpu, NesMMU& mmu)
{
	unsigned int operand = mmu.read(cpu.getPC() + 1);
	unsigned int pc = cpu.getPC();

	cpu.setOperand(operand);

	if (((pc + operand) & 0xFF00) != (pc & 0xFF00))
	{
		return 2;
	}
	return 1;
}

int ACC_m(NesCPU& cpu, NesMMU& mmu)
{
	cpu.setOperand(cpu.getA());
	return 1;
}
int IMP_m(NesCPU& cpu, NesMMU& mmu)
{
	return 0;
}

int UNI_m(NesCPU& cpu, NesMMU& mmu)
{

	cerr << "unimplemented addressing mode" << endl;
	return 0;
}

/*
 * Instruction simulation
 */

unsigned char getOffset(unsigned char off, bool flag)
{
	/*unsigned char sign = (((off & 0x80)>>8) *-1) | 0x01 ;
	 unsigned char depl = off & 0x7F;
	 depl *= sign;*/

	if (flag)
	{
		return off;
	}
	else
	{
		return 0;
	}

}

unsigned char calcOverflow(sbyte op1, sbyte op2, sbyte res)
{

	return (op1 >> 7 == op1 >> 7) && (op1 >> 7 != res >> 7);

}

void push(NesCPU& cpu, unsigned char val)
{
 return;
}

unsigned char pull(NesCPU& cpu)
{

	return 0;
}

int unimp_f(NesCPU& cpu)
{

	cerr << "unimplemented instruction" << endl;

	//assert(false);
	return 0;
}

int ADC_f(NesCPU& cpu)
{

	unsigned int res = cpu.getA();
	unsigned char op = cpu.getOperand();

	res += op;

	unsigned char carry = cpu.getC();

	res += carry;

	cout << "res:" << (int) res << endl;

	cpu.setC(res > 0x00FF);
	cpu.setN((res & 0x00FF) >> 7);
	cpu.setZ((res & 0x00FF) == 0);
	cpu.setV(calcOverflow(cpu.getA(), op, res));
	cpu.setA(res);

	return 1;
}
int AND_f(NesCPU&cpu)
{
	unsigned char res = cpu.getA() & cpu.getOperand();
	cpu.setA(res);
	cpu.setN(res >> 7);
	cpu.setZ(res == 0);
	return 1;
}

int ASL_f(NesCPU&cpu)
{

	unsigned char op = cpu.getOperand();

	cpu.setC(op & 0x80);
	op <<= 1;
	/*
	 * Here, store flag is used to signal accumulator mode,
	 * instead of signaling that a store will happen.
	 * If it's false, operand comes from memory and needs to
	 * be stored back to memory (so causes a store!).
	 *
	 */

	if (cpu.getStoreFlag())
	{
		cpu.setA(op);

	}
	else
	{
		cpu.store(op);
	}

	cpu.setN(op >> 7);
	cpu.setZ(op == 7);

	return 1;
}
int BCC_f(NesCPU&cpu)
{

	bool condition = !cpu.getC();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));
	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;
}
int BCS_f(NesCPU&cpu)
{

	bool condition = cpu.getC();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));
	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;

}
int BEQ_f(NesCPU&cpu)
{
	bool condition = cpu.getZ();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));

	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;

}
int BIT_f(NesCPU&cpu)
{

	unsigned char op = cpu.getOperand();

	cpu.setZ((cpu.getA() & op) == 0);

	cpu.setN((op & 0x80) >> 7);
	cpu.setV((op & 0x40) >> 6);

	return 1;
}
int BMI_f(NesCPU&cpu)
{
	bool condition = cpu.getN();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));
	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;

}
int BNE_f(NesCPU&cpu)
{
	bool condition = !cpu.getZ();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));
	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;

}
int BPL_f(NesCPU&cpu)
{
	bool condition = !cpu.getN();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));
	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;

}
int BRK_f(NesCPU&cpu)
{
	unsigned int pc = cpu.getPC();
	push(cpu,pc>>8);
	push(cpu,pc&0xFF);
	cpu.setB(true);
	push(cpu,cpu.getP());
	/* Need new adressing mode...*/
	//unsigned char lsb =

	return 7;
}

int BVC_f(NesCPU&cpu)
{
	bool condition = !cpu.getV();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));
	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;

}
int BVS_f(NesCPU&cpu)
{
	bool condition = cpu.getV();
	unsigned int pc = cpu.getPC();
	cpu.incrementPC(getOffset(cpu.getOperand(), condition));
	return 1+ ((pc && 0xFF00 != cpu.getPC() && 0xFF00))+condition;

}
int CLC_f(NesCPU&cpu)
{
	cpu.setC(false);
	return 1;
}
int CLD_f(NesCPU&cpu)
{
	cpu.setD(false);
	return 1;
}
int CLI_f(NesCPU&cpu)
{
	cpu.setI(false);
	return 1;
}
int CLV_f(NesCPU&cpu)
{
	cpu.setV(false);
	return 1;
}
int CMP_f(NesCPU&cpu)
{

	return 1;
}
int CPX_f(NesCPU&cpu)
{
	int16_t res = (sbyte) cpu.getX();
	sbyte op = (sbyte) cpu.getOperand();

	res -= op;

	unsigned char carry = (res & 0x0100) >> 8;
	cpu.setC(carry ^ 0x1);
	cpu.setN((res & 0x00FF) >> 7);
	cpu.setZ((res & 0x00FF) == 0);

	return 1;
}
int CPY_f(NesCPU&cpu)
{

	int16_t res = (sbyte) cpu.getY();
	sbyte op = (sbyte) cpu.getOperand();

	res -= op;

	unsigned char carry = (res & 0x0100) >> 8;

	cpu.setC(carry ^ 0x1);
	cpu.setN((res & 0x00FF) >> 7);
	cpu.setZ((res & 0x00FF) == 0);
	return 1;
}

//Problème de calcul des cycles:
//Il faut overrider les résultat intermédiaire du mode
//d'adressage (absolute,x n'a pas de différence avec le page
//boundary).
int DEC_f(NesCPU&cpu)
{

	return 1;
}
int DEX_f(NesCPU&cpu)
{

	unsigned char X = cpu.getX() - 1;
	cpu.setX(X);

	cpu.setN(X >> 7);
	cpu.setZ(X == 0);

	return 1;
}
int DEY_f(NesCPU&cpu)
{

	unsigned char Y = cpu.getY() - 1;
	cpu.setY(Y);

	cpu.setN(Y >> 7);
	cpu.setZ(Y == 0);

	return 1;
}
int EOR_f(NesCPU&cpu)
{

	return 1;
}
//Problème de calcul des cycles:
//Il faut overrider les résultat intermédiaire du mode
//d'adressage (absolute,x n'a pas de différence avec le page
//boundary).

int INC_f(NesCPU&cpu)
{

	return 1;
}
int INX_f(NesCPU&cpu)
{

	return 1;
}
int INY_f(NesCPU&cpu)
{

	return 1;
}
int JMP_f(NesCPU&cpu)
{

	return 1;
}
int JSR_f(NesCPU&cpu)
{

	return 1;
}
int LDA_f(NesCPU&cpu)
{
	cpu.setA(cpu.getOperand());
	unsigned char A = cpu.getA();

	cpu.setN(A >> 7);
	cpu.setZ(A == 0);

	return 1;
}
int LDX_f(NesCPU&cpu)
{
	cpu.setX(cpu.getOperand());
	unsigned char X = cpu.getX();

	cpu.setN(X >> 7);
	cpu.setZ(X == 0);
	return 1;
}
int LDY_f(NesCPU&cpu)
{
	cpu.setY(cpu.getOperand());
	unsigned char Y = cpu.getY();

	cpu.setN(Y >> 7);
	cpu.setZ(Y == 0);

	return 1;
}
int LSR_f(NesCPU&cpu)
{

	return 1;
}
int NOP_f(NesCPU&cpu)
{

	return 1;
}
int ORA_f(NesCPU&cpu)
{

	return 1;
}
int PHA_f(NesCPU&cpu)
{

	return 1;
}
int PHP_f(NesCPU&cpu)
{

	return 1;
}
int PLA_f(NesCPU&cpu)
{

	return 1;
}
int PLP_f(NesCPU&cpu)
{

	return 1;
}
int ROL_f(NesCPU&cpu)
{

	return 1;
}
int ROR_f(NesCPU&cpu)
{

	return 1;
}
int RTI_f(NesCPU&cpu)
{

	return 1;
}
int RTS_f(NesCPU&cpu)
{

	return 1;
}
int SBC_f(NesCPU&cpu)
{

	unsigned int res = cpu.getA();
	unsigned char op = cpu.getOperand();

	res -= op;

	unsigned char carry = !cpu.getC();

	res -= carry;

	cout << "res:" << (int) res << endl;

	cpu.setC(res > 0x00FF);
	cpu.setN((res & 0x00FF) >> 7);
	cpu.setZ((res & 0x00FF) == 0);
	cpu.setV(calcOverflow(cpu.getA(), op, res));
	cpu.setA(res);

	return 1;
}
int SEC_f(NesCPU&cpu)
{
	cpu.setC(true);
	return 1;
}
int SED_f(NesCPU&cpu)
{
	cpu.setD(true);
	return 1;
}
int SEI_f(NesCPU&cpu)
{
	cpu.setI(true);
	return 1;
}
int STA_f(NesCPU&cpu)
{

	return 1;
}
int STX_f(NesCPU&cpu)
{

	return 1;
}
int STY_f(NesCPU&cpu)
{

	return 1;
}

int TAX_f(NesCPU& cpu)
{

	unsigned char A = cpu.getA();

	cpu.setX(A);

	cpu.setN(A >> 7);
	cpu.setZ(A == 0);

	return 1;
}

int TAY_f(NesCPU& cpu)
{

	unsigned char A = cpu.getA();

	cpu.setY(A);

	cpu.setN(A >> 7);
	cpu.setZ(A == 0);

	return 1;
}

int TXA_f(NesCPU& cpu)
{

	unsigned char X = cpu.getX();

	cpu.setA(X);

	cpu.setN(X >> 7);
	cpu.setZ(X == 0);

	return 1;
}
int TSX_f(NesCPU& cpu)
{

	unsigned char S = cpu.getS();

	cpu.setX(S);

	cpu.setN(S >> 7);
	cpu.setZ(S == 0);

	return 1;
}

int TXS_f(NesCPU& cpu)
{

	unsigned char X = cpu.getX();

	cpu.setS(X);

	return 1;
}

int TYA_f(NesCPU& cpu)
{

	unsigned char Y = cpu.getY();

	cpu.setA(Y);

	cpu.setN(Y >> 7);
	cpu.setZ(Y == 0);

	return 1;
}
