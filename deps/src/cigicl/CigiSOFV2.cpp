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
 *  FILENAME:   CigiSOFV2.cpp
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
 *  LastRcvdHostFrame member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to set the LastRcvdHostFrame member variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiSOFV2.h"
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
// CigiSOFV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSOFV2::CigiSOFV2()
{

   PacketID = CIGI_SOF_PACKET_ID_V2;
   PacketSize = CIGI_SOF_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   BSwapEn = false;
   DatabaseID = 0;
   FrameCntr = 0;
   TimeStampV2 = 0.0f;
   TimeStampV3 = 0;
   ByteSwap = 0x8000;
   IGMode = Standby;
   TimestampValid = false;
   EarthRefModel = WGS84;
   LastRcvdHostFrame = 0;

}

// ================================================
// ~CigiSOFV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSOFV2::~CigiSOFV2()
{

}




// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSOFV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseSOF *Data = ( CigiBaseSOF *) Base;


   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;
   *CDta.c++ = Version;

   *CDta.b++ = Data->DatabaseID;
   *CDta.c++ = Data->IGStatus;

   Cigi_uint8 HDta = (Cigi_uint8)((Data->IGMode << 6) & 0xc0);

   *CDta.c++ = HDta;
   *CDta.c++ = 0;
   *CDta.c++ = 0;

   CIGI_SCOPY4(CDta.l++, &Data->FrameCntr);
   CIGI_SCOPY4(CDta.f++, &Data->TimeStampV2);


   return(PacketSize);

}


// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSOFV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 3;

   DatabaseID = *CDta.b++;
   IGStatus = *CDta.c++;

   Cigi_uint8 HDta = *CDta.c++;

   IGMode = (IGModeGrp)((HDta >> 6) & 0x03);

   CDta.s++;

   CIGI_SCOPY4(&FrameCntr,CDta.l++);
   CIGI_SCOPY4(&TimeStampV2,CDta.f++);

   TimeStampV3 = ((Cigi_uint32)TimeStampV2) / 10;

   LastRcvdHostFrame = FrameCntr;

   Swap = false;

   return(CIGI_SUCCESS);

}




// ====================================================================
// Accessors
// ====================================================================



// ================================================
// SetTimeStamp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSOFV2::SetTimeStamp(const float TimeStamp, bool bndchk)
{

   TimeStampV2 = TimeStamp;
   TimeStampV3 = ((Cigi_uint32)TimeStampV2) / 10;

   TimestampValid = true;

   return(CIGI_SUCCESS);

}



