//
// kernel.cpp
//
#include "kernel.h"
#include <circle/timer.h>

static const char FromKernel[] = "kernel";

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
	m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel (), &m_Timer),
	// TODO: add more member initializers here
	nes()
{
}

CKernel::~CKernel (void)
{
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	// TODO: call Initialize () of added members here (if required)

	return bOK;
}

TShutdownMode CKernel::Run (void)
{
    while(true){
	nes.Step();
	if (nes.cpu.nmiOccurred){
	    for (unsigned int i = 0; i < 256; ++i) {
		for (unsigned int j = 0; j < 240; ++j) {
		    m_Screen.SetPixel(i * 4, j * 4, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 1, j * 4, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 2, j * 4, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 3, j * 4, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4, j * 4 + 1, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 1, j * 4 + 1, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 2, j * 4 + 1, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 3, j * 4 + 1, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4, j * 4 + 2, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 1, j * 4 + 2, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 2, j * 4 + 2, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 3, j * 4 + 2, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4, j * 4 + 3, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 1, j * 4 + 3, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 2, j * 4 + 3, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
                    m_Screen.SetPixel(i * 4 + 3, j * 4 + 3, COLOR32(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue, 0));
		}
	    }
	}
    }
    return ShutdownHalt;
}
