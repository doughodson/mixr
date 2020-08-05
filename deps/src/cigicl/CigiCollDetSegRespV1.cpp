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
 *  FILENAME:   CigiCollDetSegRespV1.cpp
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
 *  Modified the class constructor to initialize the MinorVersion member 
 *  variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiCollDetSegRespV1.h"
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
// CigiCollDetSegRespV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetSegRespV1::CigiCollDetSegRespV1()
{

   PacketID = CIGI_COLL_DET_SEG_RESP_PACKET_ID_V1;
   PacketSize = CIGI_COLL_DET_SEG_RESP_PACKET_SIZE_V1;
   Version = 1;
   MinorVersion = 0;

   EntityID = 0;
   SegID = 0;
   CollType = NonEntity;
   CollEntityID = 0;
   Material = 0;
   IntersectDist = 0.0;
   X = 0.0;
   Y = 0.0;
   Z = 0.0;
   IsValidV1or2 = true;

}

// ================================================
// ~CigiCollDetSegRespV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetSegRespV1::~CigiCollDetSegRespV1()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetSegRespV1::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseCollDetSegResp * Data = ( CigiBaseCollDetSegResp *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);

   Cigi_uint8 HDta = (Data->SegID << 1) & 0xfe;
   *CDta.c++ = HDta;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   CIGI_SCOPY4(CDta.l++, &Data->Material);

   if(Data->IsValidV1or2)
   {
      CIGI_SCOPY4(CDta.f++, &Data->X);
      CIGI_SCOPY4(CDta.f++, &Data->Y);
      CIGI_SCOPY4(CDta.f++, &Data->Z);
   }
   else
   {
      float tzero = 0.0f;
      CIGI_SCOPY4(CDta.f++, &tzero);
      CIGI_SCOPY4(CDta.f++, &tzero);
      CIGI_SCOPY4(CDta.f++, &tzero);
   }


   memcpy(Buff,tBuf,CIGI_COLL_DET_SEG_RESP_PACKET_SIZE_V1);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetSegRespV1::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_COLL_DET_SEG_RESP_PACKET_SIZE_V1);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);

   SegID = (*CDta.c++ >> 1) & 0x7f;

   CDta.c += 3;

   CIGI_SCOPY4(&Material, CDta.l++);

   CIGI_SCOPY4(&X, CDta.f++);
   CIGI_SCOPY4(&Y, CDta.f++);
   CIGI_SCOPY4(&Z, CDta.f++);

   IsValidV1or2 = true;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SegID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetSegRespV1::SetSegID(const Cigi_uint8 SegIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SegIDIn < 0)||(SegIDIn > 127)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SegID",(Cigi_uint8)SegIDIn,0,127);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SegID = SegIDIn;
   return(CIGI_SUCCESS);

}


