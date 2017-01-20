//
// dwhcixferstagedata.h
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
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
#ifndef _dwhcixferstagedata_h
#define _dwhcixferstagedata_h

#include <circle/usb/usb.h>
#include <circle/usb/usbrequest.h>
#include <circle/usb/usbdevice.h>
#include <circle/usb/usbendpoint.h>
#include <circle/usb/dwhciframescheduler.h>
#include <circle/types.h>

class CDWHCITransferStageData
{
public:
	CDWHCITransferStageData (unsigned nChannel, CUSBRequest *pURB, boolean bIn, boolean bStatusStage);
	~CDWHCITransferStageData (void);

	// change status
	void TransactionComplete (u32 nStatus, u32 nPacketsLeft, u32 nBytesLeft);
	void SetSplitComplete (boolean bComplete);

	void SetState (unsigned nState);
	unsigned GetState (void) const;
	void SetSubState (unsigned nSubState);
	unsigned GetSubState (void) const;

	boolean BeginSplitCycle (void);

	// get transaction parameters
	unsigned GetChannelNumber (void) const;
	u8 GetDeviceAddress (void) const;
	boolean IsPeriodic (void) const;
	u8 GetEndpointType (void) const;
	u8 GetEndpointNumber (void) const;
	u32 GetMaxPacketSize (void) const;
	TUSBSpeed GetSpeed (void) const;

	u8 GetPID (void) const;
	boolean IsDirectionIn (void) const;
	boolean IsStatusStage (void) const;

	u32 GetDMAAddress (void) const;
	u32 GetBytesToTransfer (void) const;
	u32 GetPacketsToTransfer (void) const;

	boolean IsSplit (void) const;
	boolean IsSplitComplete (void) const;
	u8 GetHubAddress (void) const;
	u8 GetHubPortAddress (void) const;
	u8 GetSplitPosition (void) const;

	u32 GetStatusMask (void) const;

	// check status after transaction
	u32 GetTransactionStatus (void) const;
	boolean IsStageComplete (void) const;
	u32 GetResultLen (void) const;

	CUSBRequest *GetURB (void) const;
	CDWHCIFrameScheduler *GetFrameScheduler (void) const;

private:
	unsigned	 m_nChannel;			// parameters
	CUSBRequest	*m_pURB;
	boolean		 m_bIn;
	boolean		 m_bStatusStage;

	boolean		 m_bSplitTransaction;
	boolean		 m_bSplitComplete;

	CUSBDevice	*m_pDevice;			// cached from *pURB
	CUSBEndpoint	*m_pEndpoint;
	TUSBSpeed	 m_Speed;
	u32		 m_nMaxPacketSize;
	
	u32		 m_nTransferSize;
	unsigned	 m_nPackets;
	u32		 m_nBytesPerTransaction;
	unsigned	 m_nPacketsPerTransaction;
	u32		 m_nTotalBytesTransfered;

	unsigned	 m_nState;
	unsigned	 m_nSubState;
	u32		 m_nTransactionStatus;

	u32		*m_pTempBuffer;
	void		*m_pBufferPointer;

	CDWHCIFrameScheduler *m_pFrameScheduler;
};

#endif
