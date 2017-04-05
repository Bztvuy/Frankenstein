//
// kernel.cpp
//
#include "kernel.h"
static const char FromKernel[] = "kernel";
static Nes* pNes;

CKernel::CKernel(void)
: m_Screen(m_Options.GetWidth(), m_Options.GetHeight()),
m_Timer(&m_Interrupt),
m_Logger(m_Options.GetLogLevel(), &m_Timer),
m_DWHCI(&m_Interrupt, &m_Timer),
// TODO: add more member initializers here
embedded_rom(Frankenstein::StaticRom::raw, Frankenstein::StaticRom::length),
nes(embedded_rom) {
    pNes = &nes;
}

CKernel::~CKernel(void) {
}

boolean CKernel::Initialize(void) {
    boolean bOK = TRUE;

    if (bOK) {
	bOK = m_Screen.Initialize();
    }

    if (bOK) {
	bOK = m_Serial.Initialize(115200);
    }

    if (bOK) {
	CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
	if (pTarget == 0) {
	    pTarget = &m_Screen;
	}

	bOK = m_Logger.Initialize(pTarget);
    }

    if (bOK) {
	bOK = m_Interrupt.Initialize();
    }

    if (bOK) {
	bOK = m_Timer.Initialize();
    }

    if (bOK) {
	bOK = m_DWHCI.Initialize();
    }

    // TODO: call Initialize () of added members here (if required)

    return bOK;
}

TShutdownMode CKernel::Run(void) {
    m_Logger.Write(FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

    boolean bFound = FALSE;

    for (unsigned nDevice = 1; 1; nDevice++) {
	CString DeviceName;
	DeviceName.Format("upad%u", nDevice);

	CUSBGamePadDevice *pGamePad =
		(CUSBGamePadDevice *) m_DeviceNameService.GetDevice(DeviceName, FALSE);
	if (pGamePad == 0) {
	    break;
	}

	const TGamePadState *pState = pGamePad->GetReport();
	if (pState == 0) {
	    m_Logger.Write(FromKernel, LogError, "Cannot get report from %s",
		    (const char *) DeviceName);

	    continue;
	}

	m_Logger.Write(FromKernel, LogNotice, "Gamepad %u: %d Button(s) %d Hat(s)",
		nDevice, pState->nbuttons, pState->nhats);

	for (int i = 0; i < pState->naxes; i++) {
	    m_Logger.Write(FromKernel, LogNotice, "Gamepad %u: Axis %d: Minimum %d Maximum %d",
		    nDevice, i + 1, pState->axes[i].minimum, pState->axes[i].maximum);
	}

	pGamePad->RegisterStatusHandler(GamePadStatusHandler);

	bFound = TRUE;
    }

    if (!bFound) {
	m_Logger.Write(FromKernel, LogPanic, "Gamepad not found");
    }

    m_Logger.Write(FromKernel, LogNotice, "Use your gamepad controls!");

    while (true) {
	nes.Step();
	if (nes.cpu.nmiOccurred) {
	    for (unsigned int i = 0; i < 256; ++i) {
		for (unsigned int j = 0; j < 240; ++j) {
		    auto color = COLOR32(nes.ppu.front[i + 256 * j].red, nes.ppu.front[i + 256 * j].green, nes.ppu.front[i + 256 * j].blue, 0);
		    m_Screen.SetPixel(i * 4, j * 4, color);
		    m_Screen.SetPixel(i * 4 + 1, j * 4, color);
		    m_Screen.SetPixel(i * 4 + 2, j * 4, color);
		    m_Screen.SetPixel(i * 4 + 3, j * 4, color);
		    m_Screen.SetPixel(i * 4, j * 4 + 1, color);
		    m_Screen.SetPixel(i * 4 + 1, j * 4 + 1, color);
		    m_Screen.SetPixel(i * 4 + 2, j * 4 + 1, color);
		    m_Screen.SetPixel(i * 4 + 3, j * 4 + 1, color);
		    m_Screen.SetPixel(i * 4, j * 4 + 2, color);
		    m_Screen.SetPixel(i * 4 + 1, j * 4 + 2, color);
		    m_Screen.SetPixel(i * 4 + 2, j * 4 + 2, color);
		    m_Screen.SetPixel(i * 4 + 3, j * 4 + 2, color);
		    m_Screen.SetPixel(i * 4, j * 4 + 3, color);
		    m_Screen.SetPixel(i * 4 + 1, j * 4 + 3, color);
		    m_Screen.SetPixel(i * 4 + 2, j * 4 + 3, color);
		    m_Screen.SetPixel(i * 4 + 3, j * 4 + 3, color);
		}
	    }
	}
    }
    return ShutdownHalt;
}

void CKernel::GamePadStatusHandler(unsigned nDeviceIndex, const TGamePadState *pState) {
    // A: pState->buttons & 0x80
    // B: pState->buttons & 0x40
    // Select: pState->buttons & 0x10
    // Start: pState->buttons & 0x20
    // Up: !pState->axes[1].value
    // Down: pState->axes[1].value & 255
    // Left: !pState->axes[0].value
    // Right: pState->axes[0].value & 255

    pNes->pad1.buttons[0] = pState->axes[0].value == 255;
    pNes->pad1.buttons[1] = !pState->axes[0].value;
    pNes->pad1.buttons[2] = pState->axes[1].value == 255;
    pNes->pad1.buttons[3] = !pState->axes[1].value;
    pNes->pad1.buttons[4] = pState->buttons & 0x20;
    pNes->pad1.buttons[5] = pState->buttons & 0x10;
    pNes->pad1.buttons[6] = pState->buttons & 0x40;
    pNes->pad1.buttons[7] = pState->buttons & 0x80;
}