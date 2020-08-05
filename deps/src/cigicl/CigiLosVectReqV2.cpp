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
 *  FILENAME:   CigiLosVectReqV2.cpp
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
 *  Modified the Unpack method to set the UpdatePeriod member variable.
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

#include "CigiLosVectReqV2.h"
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
// CigiLosVectReqV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosVectReqV2::CigiLosVectReqV2()
{

   PacketID = CIGI_LOS_VECT_REQ_PACKET_ID_V2;
   PacketSize = CIGI_LOS_VECT_REQ_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   LosID = 0;
   ReqType = Extended;
   SrcCoordSys = Geodetic;
   ResponseCoordSys = Geodetic;
   AlphaThresh = 0;
   EntityID = 0;
   VectAz = 0.0;
   VectEl = 0.0;
   MinRange = 0.0;
   MaxRange = 0.0;
   SrcXLat = 0.0;
   SrcYLon = 0.0;
   SrcZAlt = 0.0;
   Mask = 0;
   UpdatePeriod = 0;
   ValidV1or2 = true;

}

// ================================================
// ~CigiLosVectReqV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosVectReqV2::~CigiLosVectReqV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosVectReqV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseLosVectReq * Data = ( CigiBaseLosVectReq *)Base;

   if(!Data->ValidV1or2)
      return(0);

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->LosID);

   if(Data->VectAz < 0.0f)
      Data->VectAz += 360.0f;

   CIGI_SCOPY4(CDta.f++, &Data->VectAz);

   CIGI_SCOPY4(CDta.f++, &Data->VectEl);

   *CDta.l++ = 0;

   CIGI_SCOPY4(CDta.f++, &Data->MinRange);
   CIGI_SCOPY4(CDta.f++, &Data->MaxRange);
   CIGI_SCOPY8(CDta.d++, &Data->SrcZAlt);
   CIGI_SCOPY8(CDta.d++, &Data->SrcXLat);
   CIGI_SCOPY8(CDta.d++, &Data->SrcYLon);

   memcpy(Buff,tBuf,CIGI_LOS_VECT_REQ_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosVectReqV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_LOS_VECT_REQ_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&LosID, CDta.s++);
   CIGI_SCOPY4(&VectAz, CDta.f++);
   CIGI_SCOPY4(&VectEl, CDta.f++);

   CDta.l++;

   CIGI_SCOPY4(&MinRange, CDta.f++);
   CIGI_SCOPY4(&MaxRange, CDta.f++);
   CIGI_SCOPY8(&SrcZAlt, CDta.d++);
   CIGI_SCOPY8(&SrcXLat, CDta.d++);
   CIGI_SCOPY8(&SrcYLon, CDta.d++);

   UpdatePeriod = 0;

   ValidV1or2 = true;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// VectAz
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosVectReqV2::SetVectAz(const float VectAzIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((VectAzIn < 0.0)||(VectAzIn > 360.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("VectAz",(float)VectAzIn,0.0,360.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   VectAz = VectAzIn;
   return(CIGI_SUCCESS);

}


