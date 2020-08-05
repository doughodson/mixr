/** <pre>
 *  The SDK is used to create and format CIGI compliant messages.
 *  Copyright (c) 2001-2005 The Boeing Company
 *  
 *  This library is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU Lesser General Public License as published by 
 *  the Free Software Foundation; either version 2.1 of the License, or (at 
 *  your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more 
 *  details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License 
 *  along with this library; if not, write to the Free Software Foundation, 
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *  
 *  FILENAME:   CigiIGCtrlV3_3.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface (CIGI) SDK
 *  
 *  PROGRAM DESCRIPTION: 
 *  ...
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/09/2008 Greg Basler                       Version 2.2.0
 *  Initial Release for CIGI 3.3 compatibility.
 *  Based on CigiIGCtrlV3_2.cpp"
 *  
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Corrected Version Number
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiIGCtrlV3_3.h"
#include "CigiSwapping.h"
#include "CigiVersionID.h"


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiIGCtrlV3_3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGCtrlV3_3::CigiIGCtrlV3_3()
{

   PacketID = CIGI_IG_CTRL_PACKET_ID_V3_2;
   PacketSize = CIGI_IG_CTRL_PACKET_SIZE_V3_2;
   Version = 3;
   MinorVersion = 3;

   BSwapEn = false;
   DatabaseID = 0;
   FrameCntr = 0;
   TimeStampV1 = 0;
   TimeStampV2 = 0.0f;
   TimeStampV3 = 0;
   ByteSwap = 0x8000;
   IGMode = Standby;
   TrackDeviceEn = Disable;
   BoresightTrackDevice = false;
   TimestampValid = false;
   SmoothingEn = false;
   LastRcvdIGFrame = 0;

}

// ================================================
// ~CigiIGCtrlV3_3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGCtrlV3_3::~CigiIGCtrlV3_3()
{

}


// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGCtrlV3_3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;
   CigiVersionID PackingVer = *((CigiVersionID *)Spec);

   CigiBaseIGCtrl *Data = ( CigiBaseIGCtrl *) Base;

   if(PackingVer.GetCombinedCigiVersion() < 0x0303)
      PackingVer.SetCigiVersion(3,3);

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;
   *CDta.c++ = PackingVer.CigiMajorVersion;

   *CDta.b++ = Data->DatabaseID;

   Cigi_uint8 HDta = (PackingVer.CigiMinorVersion << 4) & 0xf0;
   HDta |= (Data->SmoothingEn) ? 0x08 : 0;
   HDta |= (Data->TimestampValid) ? 0x04 : 0;
   HDta |= (Cigi_uint8)(Data->IGMode & 0x03);

   *CDta.c++ = HDta;
   *CDta.c++ = 0;
   *CDta.s++ = 0x8000;  // Byte Swap value

   *CDta.l++ = Data->FrameCntr;
   *CDta.l++ = Data->TimeStampV3;
   *CDta.l++ = Data->LastRcvdIGFrame;
   *CDta.l++ = 0;

   return(PacketSize);

}


// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGCtrlV3_3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 3;

   DatabaseID = *CDta.b++;

   Cigi_uint8 HDta = *CDta.c++;

   IGMode = (IGModeGrp)(HDta & 0x03);
   TimestampValid = ((HDta & 0x04) != 0);
   SmoothingEn = ((HDta & 0x08) != 0);

   CDta.c++;

   ByteSwap = *CDta.s++;
   BSwapEn = (ByteSwap == 0x0080);


   if(!BSwapEn)
   {
      FrameCntr = *CDta.l++;
      TimeStampV3 = *CDta.l++;
      LastRcvdIGFrame = *CDta.l++;
   }
   else
   {
      CigiSwap4(&FrameCntr, CDta.l++);
      CigiSwap4(&TimeStampV3, CDta.l++);
      CigiSwap4(&LastRcvdIGFrame, CDta.l++);
   }

   TimeStampV1 = TimeStampV3 / 10;
   TimeStampV2 = (float)TimeStampV1;



   return(CIGI_SUCCESS);

}


// ====================================================================
// Accessors
// ====================================================================



// ================================================
// SetTimeStamp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGCtrlV3_3::SetTimeStamp(const Cigi_uint32 TimeStamp, bool bndchk)
{

   TimeStampV3 = TimeStamp;
   TimeStampV1 = TimeStampV3 / 10;
   TimeStampV2 = (float)TimeStampV1;

   return(CIGI_SUCCESS);

}


