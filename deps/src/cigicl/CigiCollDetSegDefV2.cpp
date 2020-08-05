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
 *  FILENAME:   CigiCollDetSegDefV2.cpp
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

#include "CigiCollDetSegDefV2.h"
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
// CigiCollDetSegDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetSegDefV2::CigiCollDetSegDefV2()
{

   PacketID = CIGI_COLL_DET_SEG_DEF_PACKET_ID_V2;
   PacketSize = CIGI_COLL_DET_SEG_DEF_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   EntityID = 0;
   SegmentID = 0;
   SegmentEn = false;
   X1 = 0.0;
   Y1 = 0.0;
   Z1 = 0.0;
   X2 = 0.0;
   Y2 = 0.0;
   Z2 = 0.0;
   Mask = 0;

}

// ================================================
// ~CigiCollDetSegDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetSegDefV2::~CigiCollDetSegDefV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetSegDefV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseCollDetSegDef * Data = ( CigiBaseCollDetSegDef *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);

   Cigi_uint8 HDta = Data->SegmentID;
   if(HDta > 127)
      HDta = 127;

   HDta |= (Data->SegmentEn) ? 0x80 : 0x00;

   *CDta.c++ = HDta;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   CIGI_SCOPY4(CDta.l++, &Data->Mask);

   float tpt;
   Cigi_int16 stpt;
   
   tpt = Data->X1 / 0.015625f;
   if(tpt > 32767.0f)
      stpt = 32767;
   else if(tpt < -32768.0f)
      stpt = -32768;
   else
      stpt = (Cigi_int16)tpt;

   CIGI_SCOPY2(CDta.s++, &stpt);

   tpt = Data->Y1 / 0.015625f;
   if(tpt > 32767.0f)
      stpt = 32767;
   else if(tpt < -32768.0f)
      stpt = -32768;
   else
      stpt = (Cigi_int16)tpt;

   CIGI_SCOPY2(CDta.s++, &stpt);

   tpt = Data->Z1 / 0.015625f;
   if(tpt > 32767.0f)
      stpt = 32767;
   else if(tpt < -32768.0f)
      stpt = -32768;
   else
      stpt = (Cigi_int16)tpt;

   CIGI_SCOPY2(CDta.s++, &stpt);

   tpt = Data->X2 / 0.015625f;
   if(tpt > 32767.0f)
      stpt = 32767;
   else if(tpt < -32768.0f)
      stpt = -32768;
   else
      stpt = (Cigi_int16)tpt;

   CIGI_SCOPY2(CDta.s++, &stpt);

   tpt = Data->Y2 / 0.015625f;
   if(tpt > 32767.0f)
      stpt = 32767;
   else if(tpt < -32768.0f)
      stpt = -32768;
   else
      stpt = (Cigi_int16)tpt;

   CIGI_SCOPY2(CDta.s++, &stpt);

   tpt = Data->Z2 / 0.015625f;
   if(tpt > 32767.0f)
      stpt = 32767;
   else if(tpt < -32768.0f)
      stpt = -32768;
   else
      stpt = (Cigi_int16)tpt;

   CIGI_SCOPY2(CDta.s++, &stpt);


   memcpy(Buff,tBuf,CIGI_COLL_DET_SEG_DEF_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetSegDefV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_COLL_DET_SEG_DEF_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);

   Cigi_uint8 HDta = *CDta.c++;
   SegmentID = HDta & 0x7f;
   SegmentEn = ((HDta & 0x80) != 0);

   CDta.c += 3;

   CIGI_SCOPY4(&Mask, CDta.l++);

   Cigi_int16 stpt;
   float tpt;

   CIGI_SCOPY2(&stpt, CDta.s++);
   tpt = (float)stpt;
   X1 = tpt * 0.015625f;

   CIGI_SCOPY2(&stpt, CDta.s++);
   tpt = (float)stpt;
   Y1 = tpt * 0.015625f;

   CIGI_SCOPY2(&stpt, CDta.s++);
   tpt = (float)stpt;
   Z1 = tpt * 0.015625f;

   CIGI_SCOPY2(&stpt, CDta.s++);
   tpt = (float)stpt;
   X2 = tpt * 0.015625f;

   CIGI_SCOPY2(&stpt, CDta.s++);
   tpt = (float)stpt;
   Y2 = tpt * 0.015625f;

   CIGI_SCOPY2(&stpt, CDta.s++);
   tpt = (float)stpt;
   Z2 = tpt * 0.015625f;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SegmentID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetSegDefV2::SetSegmentID(const Cigi_uint8 SegmentIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SegmentIDIn < 0)||(SegmentIDIn > 127)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SegmentID",(Cigi_uint8)SegmentIDIn,0,127);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SegmentID = SegmentIDIn;
   return(CIGI_SUCCESS);

}


