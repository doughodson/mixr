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
 *  FILENAME:   CigiCollDetVolRespV2.cpp
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

#include "CigiCollDetVolRespV2.h"
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
// CigiCollDetVolRespV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolRespV2::CigiCollDetVolRespV2()
{

   PacketID = CIGI_COLL_DET_VOL_RESP_PACKET_ID_V2;
   PacketSize = CIGI_COLL_DET_VOL_RESP_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   EntityID = 0;
   VolID = 0;
   CollType = NonEntity;
   CollEntityID = 0;
   CollVolID = 0;

}

// ================================================
// ~CigiCollDetVolRespV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolRespV2::~CigiCollDetVolRespV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolRespV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseCollDetVolResp * Data = ( CigiBaseCollDetVolResp *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);

   Cigi_uint8 HDta = (Data->VolID << 1) & 0xfe;
   HDta |= (Cigi_uint16)(Data->CollType & 0x01);
   *CDta.c++ = HDta;

   *CDta.c++ = 0;

   CIGI_SCOPY2(CDta.s++, &Data->CollEntityID);

   memcpy(Buff,tBuf,CIGI_COLL_DET_VOL_RESP_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolRespV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_COLL_DET_VOL_RESP_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);

   Cigi_uint8 HDta = *CDta.c++;
   VolID = (HDta >> 1) & 0x7f;
   CollType = (CollTypeGrp)(HDta & 0x01);

   CDta.c++;

   CIGI_SCOPY2(&CollEntityID, CDta.s++);

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// VolID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolRespV2::SetVolID(const Cigi_uint8 VolIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((VolIDIn < 0)||(VolIDIn > 127)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("VolID",(Cigi_uint8)VolIDIn,0,127);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   VolID = VolIDIn;
   return(CIGI_SUCCESS);

}


