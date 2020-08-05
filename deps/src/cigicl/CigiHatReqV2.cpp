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
 *  FILENAME:   CigiHatReqV2.cpp
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
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the class constructor to initialize the MinorVersion and 
 *  UpdatePeriod member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to use the UpdatePeriod member variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiHatReqV2.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"

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
// CigiHatReqV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatReqV2::CigiHatReqV2()
{

   PacketID = CIGI_HAT_REQ_PACKET_ID_V2;
   PacketSize = CIGI_HAT_REQ_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   HatHotID = 0;
   ReqType = HAT;
   SrcCoordSys = Geodetic;
   UpdatePeriod = 0;
   EntityID = 0;
   LatOrXoff = 0.0;
   LonOrYoff = 0.0;
   AltOrZoff = 0.0;
   IsValidV1or2 = true;

}

// ================================================
// ~CigiHatReqV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatReqV2::~CigiHatReqV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatReqV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseHatHotReq * Data = ( CigiBaseHatHotReq *)Base;

   if(!Data->IsValidV1or2)
      return(0);

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->HatHotID);

   *CDta.l++ = 0;

   CIGI_SCOPY8(CDta.d++, &Data->AltOrZoff);
   CIGI_SCOPY8(CDta.d++, &Data->LatOrXoff);
   CIGI_SCOPY8(CDta.d++, &Data->LonOrYoff);

   memcpy(Buff,tBuf,CIGI_HAT_REQ_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatReqV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_HAT_REQ_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&HatHotID, CDta.s++);

   CDta.l++;

   CIGI_SCOPY8(&AltOrZoff, CDta.d++);
   CIGI_SCOPY8(&LatOrXoff, CDta.d++);
   CIGI_SCOPY8(&LonOrYoff, CDta.d++);


   SrcCoordSys = Geodetic;
   IsValidV1or2 = true;

   UpdatePeriod = 0;

   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatReqV2::GetCnvt(CigiVersionID &CnvtVersion,
                          CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   switch(CnvtVersion.CigiMajorVersion)
   {
   case 1:
      CnvtInfo.CnvtPacketID = CIGI_HAT_REQ_PACKET_ID_V1;
      break;
   case 2:
      CnvtInfo.CnvtPacketID = CIGI_HAT_REQ_PACKET_ID_V2;
      break;
   default:
      // The Packet ID for all V3 HatHotReq are the same ID
      CnvtInfo.CnvtPacketID = CIGI_HAT_HOT_REQ_PACKET_ID_V3;
      break;
   }

   return(CIGI_SUCCESS);
}



// ====================================================================
// Accessors
// ====================================================================


