//
// kernel.cpp
//
#include "kernel.h"
static const char FromKernel[] = "kernel";

Nes* CKernel::s_nes = nullptr;
CLogger* CKernel::s_logger = nullptr;
CInterruptSystem* CKernel::s_interrupt = nullptr;
TGamePadState CKernel::s_input_player1;
TGamePadState CKernel::s_input_player2;

CKernel::CKernel(void)
    : m_Screen(m_Options.GetWidth(), m_Options.GetHeight())
    , m_Timer(&m_Interrupt)
    , m_Logger(m_Options.GetLogLevel(), &m_Timer)
    , m_DWHCI(&m_Interrupt, &m_Timer)
    , embedded_rom(Frankenstein::StaticRom::raw, Frankenstein::StaticRom::length)
    , nes(embedded_rom, &m_Screen)
{
    CKernel::s_logger = &m_Logger;
    CKernel::s_interrupt = &m_Interrupt;

    CKernel::s_input_player1.axes[0].value = 0;
    CKernel::s_input_player1.axes[1].value = 0;
    CKernel::s_input_player1.buttons = 0;

    CKernel::s_input_player2.axes[0].value = 0;
    CKernel::s_input_player2.axes[1].value = 0;
    CKernel::s_input_player2.buttons = 0;
}

CKernel::~CKernel(void)
{
}

boolean CKernel::Initialize(void)
{
    boolean bOK = TRUE;

    if (bOK) {
        bOK = m_Screen.Initialize();
    }

    if (bOK) {
        bOK = m_Serial.Initialize(115200);
    }

    if (bOK) {
        CDevice* pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
        if (pTarget == 0) {
            pTarget = &m_Screen;
        }

        bOK = m_Logger.Initialize(pTarget);
        CKernel::s_logger = &m_Logger;
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

TShutdownMode CKernel::Run(void)
{
    m_Logger.Write(FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

    boolean bFound = FALSE;

    for (unsigned nDevice = 1; 1; nDevice++) {
        CUSBGamePadDevice* pGamePad = nullptr;
        const TGamePadState* pState = nullptr;
        
        CString DeviceName;
        DeviceName.Format("upad%u", nDevice);

        pGamePad = (CUSBGamePadDevice*)m_DeviceNameService.GetDevice(DeviceName, FALSE);
        if (pGamePad == nullptr) {
            break;
        }

        pState = pGamePad->GetReport();
        if (pState == 0) {
            m_Logger.Write(FromKernel, LogError, "Cannot get report from %s",
                (const char*)DeviceName);

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
            nes.pad1.buttons[Gamepad::ButtonIndex::A]      = s_input_player1.buttons & 0x80;
            nes.pad1.buttons[Gamepad::ButtonIndex::B]      = s_input_player1.buttons & 0x40;
            nes.pad1.buttons[Gamepad::ButtonIndex::Select] = s_input_player1.buttons & 0x10;
            nes.pad1.buttons[Gamepad::ButtonIndex::Start]  = s_input_player1.buttons & 0x20;
            nes.pad1.buttons[Gamepad::ButtonIndex::Up]     = !s_input_player1.axes[1].value;
            nes.pad1.buttons[Gamepad::ButtonIndex::Down]   = s_input_player1.axes[1].value == 255;
            nes.pad1.buttons[Gamepad::ButtonIndex::Left]   = !s_input_player1.axes[0].value;
            nes.pad1.buttons[Gamepad::ButtonIndex::Right]  = s_input_player1.axes[0].value == 255;

            nes.pad2.buttons[Gamepad::ButtonIndex::A]      = s_input_player2.buttons & 0x80;
            nes.pad2.buttons[Gamepad::ButtonIndex::B]      = s_input_player2.buttons & 0x40;
            nes.pad2.buttons[Gamepad::ButtonIndex::Select] = s_input_player2.buttons & 0x10;
            nes.pad2.buttons[Gamepad::ButtonIndex::Start]  = s_input_player2.buttons & 0x20;
            nes.pad2.buttons[Gamepad::ButtonIndex::Up]     = !s_input_player2.axes[1].value;
            nes.pad2.buttons[Gamepad::ButtonIndex::Down]   = s_input_player2.axes[1].value == 255;
            nes.pad2.buttons[Gamepad::ButtonIndex::Left]   = !s_input_player2.axes[0].value;
            nes.pad2.buttons[Gamepad::ButtonIndex::Right]  = s_input_player2.axes[0].value == 255;

            m_Interrupt.EnableIRQ(ARM_IRQ_USB);
        }
    }
    return ShutdownHalt;
}

void CKernel::GamePadStatusHandler(unsigned nDeviceIndex, const TGamePadState* pState)
{
    if(nDeviceIndex == 0) {
        s_input_player1 = *pState;
    }
    else if (nDeviceIndex == 1) {
        s_input_player2 = *pState;
    }
    s_interrupt->DisableIRQ(ARM_IRQ_USB);
}

