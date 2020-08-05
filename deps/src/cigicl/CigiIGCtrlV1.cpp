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
 *  FILENAME:   CigiIGCtrlV1.cpp
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
 *  09/17/2003 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Removed the inline defnitions for the PackedPointer union.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the class constructor to initialize the MinorVersion and 
 *  LastRcvdIGFrame member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to set the LastRcvdIGFrame member variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  05/09/2008 Greg Basler                       Version 2.2.0
 *  Added Extrapolation or Interpolation Smoothing flag and
 *  initialize it to false.
 *
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiIGCtrlV1.h"
#include "CigiExceptions.h"
#include "CigiSwapping.h"
#include <string.h>


#ifdef CIGI_LITTLE_ENDIAN
   #define CIGI_SCOPY2 CigiSwap2
   #define CIGI_SCOPY4 CigiSwap4
   #define CIGI_SCOPY8 CigiSwap8
#else
   #define CIGI_SCOPY2 CigiCopy2
   #define CIGI_SCOPY4 CigiCopy4
   #define CIGI_SCOPY8 CigiCopy8
#endif


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiIGCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGCtrlV1::CigiIGCtrlV1()
{

   PacketID = CIGI_IG_CTRL_PACKET_ID_V1;
   PacketSize = CIGI_IG_CTRL_PACKET_SIZE_V1;
   Version = 1;
   MinorVersion = 0;

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
// ~CigiIGCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGCtrlV1::~CigiIGCtrlV1()
{

}


// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGCtrlV1::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseIGCtrl *Data = ( CigiBaseIGCtrl *) Base;


   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;
   *CDta.c++ = Version;

   *CDta.b++ = Data->DatabaseID;

   Cigi_uint8 HDta = (Data->BoresightTrackDevice) ? 0x10 : 0;
   HDta |= (Cigi_uint8)((Data->TrackDeviceEn << 5) & 0x20);
   HDta |= (Cigi_uint8)((Data->IGMode << 6) & 0xc0);

   *CDta.c++ = HDta;
   *CDta.c++ = 0;
   *CDta.c++ = 0;
   *CDta.c++ = 0;

   CIGI_SCOPY4(CDta.l++, &Data->FrameCntr);
   CIGI_SCOPY4(CDta.l++, &Data->TimeStampV1);

   return(PacketSize);

}


// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGCtrlV1::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 3;

   DatabaseID = *CDta.b++;
   Cigi_uint8 HDta = *CDta.c;

   IGMode = (IGModeGrp)((HDta >> 6) & 0x03);
   TrackDeviceEn = (TrackDeviceEnGrp)((HDta >> 5) & 0x01);
   BoresightTrackDevice = ((HDta & 0x10) != 0);

   CDta.l++;

   CIGI_SCOPY4(&FrameCntr,CDta.l++);
   CIGI_SCOPY4(&TimeStampV1,CDta.l++);

   TimeStampV2 = (float)TimeStampV1;
   TimeStampV3 = TimeStampV1 / 10;

   LastRcvdIGFrame = FrameCntr;

   Swap = false;

   return(CIGI_SUCCESS);

}


// ====================================================================
// Accessors
// ====================================================================



// ================================================
// SetTimeStamp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGCtrlV1::SetTimeStamp(const Cigi_uint32 TimeStamp, bool bndchk)
{

   TimeStampV1 = TimeStamp;
   TimeStampV2 = (float)TimeStamp;
   TimeStampV3 = TimeStampV1 / 10;

   TimestampValid = true;

   return(CIGI_SUCCESS);

}


// ================================================
// SetTrackDeviceEn
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGCtrlV1::SetTrackDeviceEn(const TrackDeviceEnGrp TrackDeviceEnIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((TrackDeviceEnIn < 0)||(TrackDeviceEnIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("TrackDeviceEn",TrackDeviceEnIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   TrackDeviceEn = TrackDeviceEnIn;

   return(CIGI_SUCCESS);

}

