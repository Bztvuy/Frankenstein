//
// usbdevice.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014-2016  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <circle/usb/usbdevice.h>
#include <circle/usb/usbhostcontroller.h>
#include <circle/usb/usbendpoint.h>
#include <circle/usb/usbdevicefactory.h>
#include <circle/util.h>
#include <circle/sysconfig.h>
#include <circle/debug.h>
#include <assert.h>

#define MAX_CONFIG_DESC_SIZE		512		// best guess

static const char FromDevice[] = "usbdev";

u8 CUSBDevice::s_ucNextAddress = USB_FIRST_DEDICATED_ADDRESS;

CUSBDevice::CUSBDevice (CUSBHostController *pHost, TUSBSpeed Speed,
			boolean bSplitTransfer, u8 ucHubAddress, u8 ucHubPortNumber)
:	m_pHost (pHost),
	m_ucAddress (USB_DEFAULT_ADDRESS),
	m_Speed (Speed),
	m_pEndpoint0 (0),
	m_bSplitTransfer (bSplitTransfer),
	m_ucHubAddress (ucHubAddress),
	m_ucHubPortNumber (ucHubPortNumber),
	m_pDeviceDesc (0),
	m_pConfigDesc (0),
	m_pConfigParser (0)
{
	assert (m_pHost != 0);

	assert (m_pEndpoint0 == 0);
	m_pEndpoint0 = new CUSBEndpoint (this);
	assert (m_pEndpoint0 != 0);
	
	assert (ucHubPortNumber >= 1);

	for (unsigned nFunction = 0; nFunction < USBDEV_MAX_FUNCTIONS; nFunction++)
	{
		m_pFunction[nFunction] = 0;
	}
}

CUSBDevice::~CUSBDevice (void)
{
	for (unsigned nFunction = 0; nFunction < USBDEV_MAX_FUNCTIONS; nFunction++)
	{
		delete (m_pFunction[nFunction]);
		m_pFunction[nFunction] = 0;
	}

	delete m_pConfigParser;
	m_pConfigParser = 0;

	delete m_pConfigDesc;
	m_pConfigDesc = 0;
	
	delete m_pDeviceDesc;
	m_pDeviceDesc = 0;
	
	delete m_pEndpoint0;
	m_pEndpoint0 = 0;
	
	m_pHost = 0;
}

boolean CUSBDevice::Initialize (void)
{
#ifdef REALTIME
	if (m_Speed != USBSpeedHigh)
	{
		LogWrite (LogWarning, "Device speed is not allowed with REALTIME");

		return FALSE;
	}
#endif

	assert (m_pDeviceDesc == 0);
	m_pDeviceDesc = new TUSBDeviceDescriptor;
	assert (m_pDeviceDesc != 0);
	
	assert (m_pHost != 0);
	assert (m_pEndpoint0 != 0);
	
	assert (sizeof *m_pDeviceDesc >= USB_DEFAULT_MAX_PACKET_SIZE);
	if (m_pHost->GetDescriptor (m_pEndpoint0,
				    DESCRIPTOR_DEVICE, DESCRIPTOR_INDEX_DEFAULT,
				    m_pDeviceDesc, USB_DEFAULT_MAX_PACKET_SIZE)
	    != USB_DEFAULT_MAX_PACKET_SIZE)
	{
		LogWrite (LogError, "Cannot get device descriptor (short)");

		delete m_pDeviceDesc;
		m_pDeviceDesc = 0;

		return FALSE;
	}

	if (   m_pDeviceDesc->bLength	      != sizeof *m_pDeviceDesc
	    || m_pDeviceDesc->bDescriptorType != DESCRIPTOR_DEVICE)
	{
		LogWrite (LogError, "Invalid device descriptor");

		delete m_pDeviceDesc;
		m_pDeviceDesc = 0;

		return FALSE;
	}

	m_pEndpoint0->SetMaxPacketSize (m_pDeviceDesc->bMaxPacketSize0);

	if (m_pHost->GetDescriptor (m_pEndpoint0,
				    DESCRIPTOR_DEVICE, DESCRIPTOR_INDEX_DEFAULT,
				    m_pDeviceDesc, sizeof *m_pDeviceDesc)
	    != (int) sizeof *m_pDeviceDesc)
	{
		LogWrite (LogError, "Cannot get device descriptor");

		delete m_pDeviceDesc;
		m_pDeviceDesc = 0;

		return FALSE;
	}

#ifndef NDEBUG
	//debug_hexdump (m_pDeviceDesc, sizeof *m_pDeviceDesc, FromDevice);
#endif
	
	u8 ucAddress = s_ucNextAddress++;
	if (ucAddress > USB_MAX_ADDRESS)
	{
		LogWrite (LogError, "Too many devices");

		return FALSE;
	}

	if (!m_pHost->SetAddress (m_pEndpoint0, ucAddress))
	{
		LogWrite (LogError, "Cannot set address %u", (unsigned) ucAddress);

		return FALSE;
	}
	
	SetAddress (ucAddress);

	assert (m_pConfigDesc == 0);
	m_pConfigDesc = new TUSBConfigurationDescriptor;
	assert (m_pConfigDesc != 0);

	if (m_pHost->GetDescriptor (m_pEndpoint0,
				    DESCRIPTOR_CONFIGURATION, DESCRIPTOR_INDEX_DEFAULT,
				    m_pConfigDesc, sizeof *m_pConfigDesc)
	    != (int) sizeof *m_pConfigDesc)
	{
		LogWrite (LogError, "Cannot get configuration descriptor (short)");

		delete m_pConfigDesc;
		m_pConfigDesc = 0;

		return FALSE;
	}

	if (   m_pConfigDesc->bLength         != sizeof *m_pConfigDesc
	    || m_pConfigDesc->bDescriptorType != DESCRIPTOR_CONFIGURATION
	    || m_pConfigDesc->wTotalLength    >  MAX_CONFIG_DESC_SIZE)
	{
		LogWrite (LogError, "Invalid configuration descriptor");
		
		delete m_pConfigDesc;
		m_pConfigDesc = 0;

		return FALSE;
	}

	unsigned nTotalLength = m_pConfigDesc->wTotalLength;

	delete m_pConfigDesc;

	m_pConfigDesc = (TUSBConfigurationDescriptor *) new u8[nTotalLength];
	assert (m_pConfigDesc != 0);

	if (m_pHost->GetDescriptor (m_pEndpoint0,
				    DESCRIPTOR_CONFIGURATION, DESCRIPTOR_INDEX_DEFAULT,
				    m_pConfigDesc, nTotalLength)
	    != (int) nTotalLength)
	{
		LogWrite (LogError, "Cannot get configuration descriptor");

		delete m_pConfigDesc;
		m_pConfigDesc = 0;

		return FALSE;
	}

#ifndef NDEBUG
	//debug_hexdump (m_pConfigDesc, nTotalLength, FromDevice);
#endif

	assert (m_pConfigParser == 0);
	m_pConfigParser = new CUSBConfigurationParser (m_pConfigDesc, nTotalLength);
	assert (m_pConfigParser != 0);

	if (!m_pConfigParser->IsValid ())
	{
		ConfigurationError (FromDevice);

		return FALSE;
	}

	CString *pNames = GetNames ();
	assert (pNames != 0);
	LogWrite (LogNotice, "Device %s found", (const char *) *pNames);
	delete pNames;

	unsigned nFunction = 0;
	u8 ucInterfaceNumber = 0;

	TUSBInterfaceDescriptor *pInterfaceDesc;
	while ((pInterfaceDesc = (TUSBInterfaceDescriptor *) m_pConfigParser->GetDescriptor (DESCRIPTOR_INTERFACE)) != 0)
	{
		if (   pInterfaceDesc->bInterfaceNumber != ucInterfaceNumber
		    && pInterfaceDesc->bInterfaceNumber != ucInterfaceNumber+1)
		{
			LogWrite (LogDebug, "Alternate setting %u ignored",
				  (unsigned) pInterfaceDesc->bAlternateSetting);

			if (pInterfaceDesc->bInterfaceNumber == ucInterfaceNumber+1)
			{
				ucInterfaceNumber++;
			}

			continue;
		}

		assert (m_pConfigParser != 0);
		assert (m_pFunction[nFunction] == 0);
		m_pFunction[nFunction] = new CUSBFunction (this, m_pConfigParser);
		assert (m_pFunction[nFunction] != 0);

		if (!m_pFunction[nFunction]->Initialize ())
		{
			LogWrite (LogError, "Cannot initialize function");

			delete m_pFunction[nFunction];
			m_pFunction[nFunction] = 0;

			continue;
		}

		CUSBFunction *pChild = 0;

		if (nFunction == 0)
		{
			pChild = CUSBDeviceFactory::GetDevice (m_pFunction[nFunction], GetName (DeviceNameVendor));
			if (pChild == 0)
			{
				pChild = CUSBDeviceFactory::GetDevice (m_pFunction[nFunction], GetName (DeviceNameDevice));
			}
		}

		if (pChild == 0)
		{
			CString *pName = m_pFunction[nFunction]->GetInterfaceName ();
			assert (pName != 0);
			if (pName->Compare ("unknown") != 0)
			{
				LogWrite (LogNotice, "Interface %s found", (const char *) *pName);

				pChild = CUSBDeviceFactory::GetDevice (m_pFunction[nFunction], pName);
			}
			else
			{
				delete pName;
			}
		}

		delete m_pFunction[nFunction];
		m_pFunction[nFunction] = 0;

		if (pChild == 0)
		{
			LogWrite (LogWarning, "Function is not supported");

			continue;
		}

		m_pFunction[nFunction] = pChild;

		if (++nFunction == USBDEV_MAX_FUNCTIONS)
		{
			LogWrite (LogWarning, "Too many functions per device");

			break;
		}

		ucInterfaceNumber++;
	}

	if (nFunction == 0)
	{
		LogWrite (LogWarning, "Device has no supported function");

		return FALSE;
	}

	return TRUE;
}

boolean CUSBDevice::Configure (void)
{
	assert (m_pHost != 0);
	assert (m_pEndpoint0 != 0);

	if (m_pConfigDesc == 0)		// not initialized
	{
		return FALSE;
	}

	if (!m_pHost->SetConfiguration (m_pEndpoint0, m_pConfigDesc->bConfigurationValue))
	{
		LogWrite (LogError, "Cannot set configuration (%u)", (unsigned) m_pConfigDesc->bConfigurationValue);

		return FALSE;
	}

	boolean bResult = FALSE;
	
	for (unsigned nFunction = 0; nFunction < USBDEV_MAX_FUNCTIONS; nFunction++)
	{
		if (m_pFunction[nFunction] != 0)
		{
			if (!m_pFunction[nFunction]->Configure ())
			{
				//LogWrite (LogError, "Cannot configure device");

				delete m_pFunction[nFunction];
				m_pFunction[nFunction] = 0;
			}
			else
			{
				bResult = TRUE;
			}
		}
	}
	
	return bResult;
}

CString *CUSBDevice::GetName (TDeviceNameSelector Selector) const
{
	CString *pString = new CString;
	assert (pString != 0);
	
	switch (Selector)
	{
	case DeviceNameVendor:
		assert (m_pDeviceDesc != 0);
		pString->Format ("ven%x-%x",
				 (unsigned) m_pDeviceDesc->idVendor,
				 (unsigned) m_pDeviceDesc->idProduct);
		break;
		
	case DeviceNameDevice:
		assert (m_pDeviceDesc != 0);
		if (   m_pDeviceDesc->bDeviceClass == 0
		    || m_pDeviceDesc->bDeviceClass == 0xFF)
		{
			goto unknown;
		}
		pString->Format ("dev%x-%x-%x",
				 (unsigned) m_pDeviceDesc->bDeviceClass,
				 (unsigned) m_pDeviceDesc->bDeviceSubClass,
				 (unsigned) m_pDeviceDesc->bDeviceProtocol);
		break;
		
	default:
		assert (0);
	unknown:
		*pString = "unknown";
		break;
	}
	
	return pString;
}

CString *CUSBDevice::GetNames (void) const
{
	CString *pResult = new CString;
	assert (pResult != 0);

	for (unsigned nSelector = DeviceNameVendor; nSelector < DeviceNameUnknown; nSelector++)
	{
		CString *pName = GetName ((TDeviceNameSelector) nSelector);
		assert (pName != 0);

		if (pName->Compare ("unknown") != 0)
		{
			if (pResult->GetLength () > 0)
			{
				pResult->Append (", ");
			}

			pResult->Append (*pName);
		}
		
		delete pName;
	}

	if (pResult->GetLength () == 0)
	{
		*pResult = "unknown";
	}

	return pResult;
}

void CUSBDevice::SetAddress (u8 ucAddress)
{
	assert (ucAddress <= USB_MAX_ADDRESS);
	m_ucAddress = ucAddress;

	//LogWrite (LogDebug, "Device address set to %u", (unsigned) m_ucAddress);
}

u8 CUSBDevice::GetAddress (void) const
{
	return m_ucAddress;
}

TUSBSpeed CUSBDevice::GetSpeed (void) const
{
	return m_Speed;
}

boolean CUSBDevice::IsSplit (void) const
{
	return m_bSplitTransfer;
}

u8 CUSBDevice::GetHubAddress (void) const
{
	return m_ucHubAddress;
}

u8 CUSBDevice::GetHubPortNumber (void) const
{
	return m_ucHubPortNumber;
}

CUSBEndpoint *CUSBDevice::GetEndpoint0 (void) const
{
	assert (m_pEndpoint0 != 0);
	return m_pEndpoint0;
}

CUSBHostController *CUSBDevice::GetHost (void) const
{
	assert (m_pHost != 0);
	return m_pHost;
}

const TUSBDeviceDescriptor *CUSBDevice::GetDeviceDescriptor (void) const
{
	assert (m_pDeviceDesc != 0);
	return m_pDeviceDesc;
}

const TUSBConfigurationDescriptor *CUSBDevice::GetConfigurationDescriptor (void) const
{
	assert (m_pConfigDesc != 0);
	return m_pConfigDesc;
}

const TUSBDescriptor *CUSBDevice::GetDescriptor (u8 ucType)
{
	assert (m_pConfigParser != 0);
	return m_pConfigParser->GetDescriptor (ucType);
}

void CUSBDevice::ConfigurationError (const char *pSource) const
{
	assert (m_pConfigParser != 0);
	m_pConfigParser->Error (pSource);
}

void CUSBDevice::LogWrite (TLogSeverity Severity, const char *pMessage, ...)
{
	assert (pMessage != 0);

	CString Source;
	Source.Format ("%s%u-%u", FromDevice, (unsigned) m_ucHubAddress, (unsigned) m_ucHubPortNumber);

	va_list var;
	va_start (var, pMessage);

	CLogger::Get ()->WriteV (Source, Severity, pMessage, var);

	va_end (var);
}
