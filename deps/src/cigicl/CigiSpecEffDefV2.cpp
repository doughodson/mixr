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
 *  FILENAME:   CigiSpecEffDefV2.cpp
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

#include "CigiSpecEffDefV2.h"
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
// CigiSpecEffDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSpecEffDefV2::CigiSpecEffDefV2()
{

   PacketID = CIGI_SPEC_EFF_DEF_PACKET_ID_V2;
   PacketSize = CIGI_SPEC_EFF_DEF_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   EntityID = 0;
   SeqDir = Forward;
   ColorEn = false;
   Red = 0;
   Green = 0;
   Blue = 0;
   XScale = 0.0;
   YScale = 0.0;
   ZScale = 0.0;
   TimeScale = 0.0;
   EffectCnt = 0;
   Separation = 0.0;
   BurstRate = 0.0;
   Duration = 0.0;

}

// ================================================
// ~CigiSpecEffDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSpecEffDefV2::~CigiSpecEffDefV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSpecEffDefV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseSpecEffDef * Data = ( CigiBaseSpecEffDef *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.h++, &Data->EntityID);

   Cigi_uint8 HDta = (Data->SeqDir << 7) & 0x80;
   HDta |= (Data->ColorEn) ? 0x40 : 0;
   *CDta.c++ = HDta;

   *CDta.c++ = Data->Red;
   *CDta.c++ = Data->Green;
   *CDta.c++ = Data->Blue;

   float tfScl = (float)Data->XScale / 0.015625f;
   Cigi_int16 tsScl = (short)tfScl;
   CIGI_SCOPY2(CDta.h++, &tsScl);

   tfScl = Data->YScale / 0.015625f;
   tsScl = (Cigi_int16)tfScl;
   CIGI_SCOPY2(CDta.h++, &tsScl);

   tfScl = Data->ZScale / 0.015625f;
   tsScl = (Cigi_int16)tfScl;
   CIGI_SCOPY2(CDta.h++, &tsScl);

   tfScl = Data->TimeScale / 0.015625f;
   tsScl = (Cigi_int16)tfScl;
   CIGI_SCOPY2(CDta.h++, &tsScl);

   *CDta.h++ = 0;

   CIGI_SCOPY2(CDta.s++, &Data->EffectCnt);

   CIGI_SCOPY4(CDta.f++, &Data->Separation);
   CIGI_SCOPY4(CDta.f++, &Data->BurstRate);
   CIGI_SCOPY4(CDta.f++, &Data->Duration);

   memcpy(Buff,tBuf,CIGI_SPEC_EFF_DEF_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSpecEffDefV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_SPEC_EFF_DEF_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.h++);

   Cigi_uint8 HDta = *CDta.c++;
   SeqDir = (SeqDirGrp)((HDta >> 7) & 0x01);
   ColorEn = ((HDta & 0x40) != 0);

   Red = *CDta.c++;
   Green = *CDta.c++;
   Blue = *CDta.c++;

   Cigi_int16 tsScl;

   CIGI_SCOPY2(&tsScl, CDta.h++);
   float tfScl = (float)tsScl;
   XScale = tfScl * 0.015625f;

   CIGI_SCOPY2(&tsScl, CDta.h++);
   tfScl = (float)tsScl;
   YScale = tfScl * 0.015625f;

   CIGI_SCOPY2(&tsScl, CDta.h++);
   tfScl = (float)tsScl;
   ZScale = tfScl * 0.015625f;

   CIGI_SCOPY2(&tsScl, CDta.h++);
   tfScl = (float)tsScl;
   TimeScale = tfScl * 0.015625f;


   CDta.h++;

   CIGI_SCOPY2(&EffectCnt, CDta.h++);

   CIGI_SCOPY4(&Separation, CDta.f++);
   CIGI_SCOPY4(&BurstRate, CDta.f++);
   CIGI_SCOPY4(&Duration, CDta.f++);

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


