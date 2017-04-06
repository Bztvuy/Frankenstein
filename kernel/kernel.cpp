//
// kernel.cpp
//
#include "kernel.h"
static const char FromKernel[] = "kernel";

Nes* CKernel::s_nes = nullptr;
CLogger* CKernel::s_logger = nullptr;
TGamePadState CKernel::s_input;

CKernel::CKernel(void)
    : m_Screen(m_Options.GetWidth(), m_Options.GetHeight())
    , m_Timer(&m_Interrupt)
    , m_Logger(m_Options.GetLogLevel(), &m_Timer)
    , m_DWHCI(&m_Interrupt, &m_Timer)
    , embedded_rom(Frankenstein::StaticRom::raw, Frankenstein::StaticRom::length)
    , nes(embedded_rom)
{
    CKernel::s_logger = &m_Logger;

    CKernel::s_input.axes[0].value = 0;
    CKernel::s_input.axes[1].value = 0;
    CKernel::s_input.buttons = 0;
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
    
    unsigned char i = 0;
    while (true) {
        nes.Step();
        if (nes.cpu.nmiOccurred) {
            m_Interrupt.DisableIRQ(ARM_IRQ_USB);
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
            if(i == 125) {
                m_Interrupt.EnableIRQ(ARM_IRQ_USB);
            }
        }
        else if(i == 125) {
            m_Interrupt.EnableIRQ(ARM_IRQ_USB);
        }
        else if (i == 126) {
            m_Interrupt.DisableIRQ(ARM_IRQ_USB);
        }
        i++;
        nes.pad1.buttons[Gamepad::ButtonIndex::A] = s_input.buttons & 0x80;
        nes.pad1.buttons[Gamepad::ButtonIndex::B] = s_input.buttons & 0x40;
        nes.pad1.buttons[Gamepad::ButtonIndex::Select] = s_input.buttons & 0x10;
        nes.pad1.buttons[Gamepad::ButtonIndex::Start] = s_input.buttons & 0x20;
        nes.pad1.buttons[Gamepad::ButtonIndex::Up] = !s_input.axes[1].value;
        nes.pad1.buttons[Gamepad::ButtonIndex::Down] = s_input.axes[1].value == 255;
        nes.pad1.buttons[Gamepad::ButtonIndex::Left] = !s_input.axes[0].value;
        nes.pad1.buttons[Gamepad::ButtonIndex::Right] = s_input.axes[0].value == 255;
    }
    return ShutdownHalt;
}

void CKernel::GamePadStatusHandler(unsigned nDeviceIndex, const TGamePadState* pState)
{
    s_input = *pState;
}
