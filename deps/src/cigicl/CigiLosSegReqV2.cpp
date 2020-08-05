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
 *  FILENAME:   CigiLosSegReqV2.cpp
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
 *  Modified the class constructor to initialize the MinorVersion, 
 *  DestEntityIDValid, UpdatePeriod, and DestEntityID member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to set the DestEntityIDValid, UpdatePeriod, and 
 *  DestEntityID member variables.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  04/24/2007 Greg Basler                       Version 1.7.5
 *  Changed the type of request from Basic to Extended.  The Extended
 *  Response in V3 and later has the needed data for V1 & V2 responses.
 *  The Basic responses do not.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiLosSegReqV2.h"
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
// CigiLosSegReqV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosSegReqV2::CigiLosSegReqV2()
{

   PacketID = CIGI_LOS_SEG_REQ_PACKET_ID_V2;
   PacketSize = CIGI_LOS_SEG_REQ_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   LosID = 0;
   ReqType = Extended;
   SrcCoordSys = Geodetic;
   DstCoordSys = Geodetic;
   ResponseCoordSys = Geodetic;
   AlphaThresh = 0;
   EntityID = 0;
   SrcXLat = 0.0;
   SrcYLon = 0.0;
   SrcZAlt = 0.0;
   DstXLat = 0.0;
   DstYLon = 0.0;
   DstZAlt = 0.0;
   Mask = 0;
   DestEntityIDValid = false;
   UpdatePeriod = 0;
   DestEntityID = 0;
   ValidV1or2 = true;

}

// ================================================
// ~CigiLosSegReqV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosSegReqV2::~CigiLosSegReqV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[7];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseLosSegReq * Data = ( CigiBaseLosSegReq *)Base;

   // At this point return without unpacking if the
   //  data is not valid for CIGI 1 or 2.
   if(!Data->ValidV1or2)
      return(0);

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->LosID);
   *CDta.l++ = 0;
   CIGI_SCOPY8(CDta.d++, &Data->SrcZAlt);
   CIGI_SCOPY8(CDta.d++, &Data->SrcXLat);
   CIGI_SCOPY8(CDta.d++, &Data->SrcYLon);
   CIGI_SCOPY8(CDta.d++, &Data->DstZAlt);
   CIGI_SCOPY8(CDta.d++, &Data->DstXLat);
   CIGI_SCOPY8(CDta.d++, &Data->DstYLon);

   memcpy(Buff,tBuf,CIGI_LOS_SEG_REQ_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[7];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_LOS_SEG_REQ_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&LosID, CDta.s++);
   CDta.c += 4;
   CIGI_SCOPY8(&SrcZAlt, CDta.d++);
   CIGI_SCOPY8(&SrcXLat, CDta.d++);
   CIGI_SCOPY8(&SrcYLon, CDta.d++);
   CIGI_SCOPY8(&DstZAlt, CDta.d++);
   CIGI_SCOPY8(&DstXLat, CDta.d++);
   CIGI_SCOPY8(&DstYLon, CDta.d++);


   DestEntityIDValid = false;
   UpdatePeriod = 0;
   DestEntityID = 0;

   ValidV1or2 = true;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


