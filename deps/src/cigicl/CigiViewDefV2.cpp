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
 *  FILENAME:   CigiViewDefV2.cpp
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

#include "CigiViewDefV2.h"
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
// CigiViewDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiViewDefV2::CigiViewDefV2()
{

   PacketID = CIGI_VIEW_DEF_PACKET_ID_V2;
   PacketSize = CIGI_VIEW_DEF_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   ViewID = 0;
   GroupID = 0;
   FOVNearEn = false;
   FOVFarEn = false;
   FOVLeftEn = false;
   FOVRightEn = false;
   FOVTopEn = false;
   FOVBottomEn = false;
   MirrorMode = MirrorNone;
   PixelReplicateMode = ReplicateNone;
   ProjectionType = Perspective;
   Reorder = false;
   ViewType = 0;
   TrackerAssigned = false;
   FOVNear = 0.0;
   FOVFar = 0.0;
   FOVLeft = 0.0;
   FOVRight = 0.0;
   FOVTop = 0.0;
   FOVBottom = 0.0;

}

// ================================================
// ~CigiViewDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiViewDefV2::~CigiViewDefV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewDefV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseViewDef * Data = ( CigiBaseViewDef *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   Cigi_uint8 HDta = 0;
   if(Data->ViewID < 32)
      HDta = (Cigi_uint8)(Data->ViewID << 3);
   else
      HDta = (Cigi_uint8)0xf8;

   if(Data->GroupID < 8)
      HDta |= (Cigi_uint8)Data->GroupID;
   else
      HDta |= 7;

   *CDta.c++ = HDta;

   HDta = (Data->ViewType << 5) & 0xe0;
   HDta |= (Cigi_uint8)((Data->PixelReplicateMode << 2) & 0x1c);
   HDta |= (Cigi_uint8)(Data->MirrorMode & 0x03);

   *CDta.c++ = HDta;

   HDta = (Data->TrackerAssigned) ? 0x80 : 0;

   if(Data->FOVNearEn)
      HDta |= 0x40;

   if(Data->FOVFarEn)
      HDta |= 0x20;

   if(Data->FOVLeftEn)
      HDta |= 0x10;

   if(Data->FOVRightEn)
      HDta |= 0x08;

   if(Data->FOVTopEn)
      HDta |= 0x04;

   if(Data->FOVBottomEn)
      HDta |= 0x02;

   *CDta.c++ = HDta;
   *CDta.c++ = 0;
   *CDta.c++ = 0;
   *CDta.c++ = 0;

   CIGI_SCOPY4(CDta.f++, &Data->FOVNear);
   CIGI_SCOPY4(CDta.f++, &Data->FOVFar);
   CIGI_SCOPY4(CDta.f++, &Data->FOVLeft);
   CIGI_SCOPY4(CDta.f++, &Data->FOVRight);
   CIGI_SCOPY4(CDta.f++, &Data->FOVTop);
   CIGI_SCOPY4(CDta.f++, &Data->FOVBottom);

   memcpy(Buff,tBuf,CIGI_VIEW_DEF_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewDefV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_VIEW_DEF_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   Cigi_uint8 HDta = *CDta.c++;

   ViewID = (Cigi_uint8)((HDta >> 3) & 0x1f);
   GroupID = (Cigi_uint8)(HDta & 0x07);

   HDta = *CDta.c++;

   ViewType = (Cigi_uint8)((HDta >> 5) & 0x07);
   PixelReplicateMode = (PixelReplicateModeGrp)((HDta >> 2) & 0x07);
   MirrorMode = (MirrorModeGrp)(HDta & 0x03);

   HDta = *CDta.c++;
   
   TrackerAssigned = ((HDta & 0x80) != 0);
   FOVNearEn = ((HDta & 0x40) != 0);
   FOVFarEn = ((HDta & 0x20) != 0);
   FOVLeftEn = ((HDta & 0x10) != 0);
   FOVRightEn = ((HDta & 0x08) != 0);
   FOVTopEn = ((HDta & 0x04) != 0);
   FOVBottomEn = ((HDta & 0x02) != 0);

   CDta.c += 3;

   CIGI_SCOPY4(&FOVNear, CDta.f++);
   CIGI_SCOPY4(&FOVFar, CDta.f++);
   CIGI_SCOPY4(&FOVLeft, CDta.f++);
   CIGI_SCOPY4(&FOVRight, CDta.f++);
   CIGI_SCOPY4(&FOVTop, CDta.f++);
   CIGI_SCOPY4(&FOVBottom, CDta.f++);

   return(PacketSize);


}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// View ID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewDefV2::SetViewID(const Cigi_uint8 ViewIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ViewIDIn < 0)||(ViewIDIn > 31)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ViewID",(Cigi_uint16)ViewIDIn,0,31);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ViewID = (Cigi_uint16)ViewIDIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Group ID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewDefV2::SetGroupID(const Cigi_uint8 GroupIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((GroupIDIn < 0)||(GroupIDIn > 7)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("GroupID",(Cigi_uint16)GroupIDIn,0,7);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   GroupID = GroupIDIn;
   return(CIGI_SUCCESS);

}

