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
 *  FILENAME:   CigiViewDefV3.cpp
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

#include "CigiViewDefV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiViewDefV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiViewDefV3::CigiViewDefV3()
{

   PacketID = CIGI_VIEW_DEF_PACKET_ID_V3;
   PacketSize = CIGI_VIEW_DEF_PACKET_SIZE_V3;
   Version = 3;
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
// ~CigiViewDefV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiViewDefV3::~CigiViewDefV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewDefV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseViewDef * Data = ( CigiBaseViewDef *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->ViewID;
   *CDta.c++ = Data->GroupID;

   Cigi_uint8 HDta = (Cigi_uint8)((Data->MirrorMode << 6) & 0xc0);

   if(Data->FOVBottomEn)
      HDta |= 0x20;

   if(Data->FOVTopEn)
      HDta |= 0x10;

   if(Data->FOVRightEn)
      HDta |= 0x08;

   if(Data->FOVLeftEn)
      HDta |= 0x04;

   if(Data->FOVFarEn)
      HDta |= 0x02;

   if(Data->FOVNearEn)
      HDta |= 0x01;

   *CDta.c++ = HDta;

   HDta = (Cigi_uint8)((Data->ViewType << 5) & 0xe0);

   if(Data->Reorder)
      HDta |= 0x10;

   if(Data->ProjectionType == Orthographic)
      HDta |= 0x08;

   HDta |= (Cigi_uint8)(Data->PixelReplicateMode & 0x07);

   *CDta.c++ = HDta;
   *CDta.c++ = 0;

   *CDta.f++ = Data->FOVNear;
   *CDta.f++ = Data->FOVFar;
   *CDta.f++ = Data->FOVLeft;
   *CDta.f++ = Data->FOVRight;
   *CDta.f++ = Data->FOVTop;
   *CDta.f++ = Data->FOVBottom;

   return(CIGI_VIEW_DEF_PACKET_SIZE_V3);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewDefV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Skip over packet id and size;


   if(!Swap)
      ViewID = *CDta.s++;
   else
      CigiSwap2(&ViewID, CDta.s++);

   GroupID = *CDta.c++;

   Cigi_uint8 HDta = *CDta.c++;

   MirrorMode = (MirrorModeGrp)((HDta >> 6) & 0x03);
   FOVBottomEn = ((HDta & 0x20) != 0);
   FOVTopEn = ((HDta & 0x10) != 0);
   FOVRightEn = ((HDta & 0x08) != 0);
   FOVLeftEn = ((HDta & 0x04) != 0);
   FOVFarEn = ((HDta & 0x02) != 0);
   FOVNearEn = ((HDta & 0x01) != 0);

   HDta = *CDta.c++;

   ViewType = (HDta >> 5) & 0x07;
   Reorder = ((HDta & 0x10) != 0);
   ProjectionType = (ProjectionTypeGrp)((HDta >> 3) & 0x01);
   PixelReplicateMode = (PixelReplicateModeGrp)(HDta & 0x07);

   CDta.c++;

   if(!Swap)
   {
      FOVNear = *CDta.f++;
      FOVFar = *CDta.f++;
      FOVLeft = *CDta.f++;
      FOVRight = *CDta.f++;
      FOVTop = *CDta.f++;
      FOVBottom = *CDta.f++;
   }
   else
   {
      CigiSwap4(&FOVNear, CDta.f++);
      CigiSwap4(&FOVFar, CDta.f++);
      CigiSwap4(&FOVLeft, CDta.f++);
      CigiSwap4(&FOVRight, CDta.f++);
      CigiSwap4(&FOVTop, CDta.f++);
      CigiSwap4(&FOVBottom, CDta.f++);
   }

   return(CIGI_VIEW_DEF_PACKET_SIZE_V3);


}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// ProjectionType
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewDefV3::SetProjectionType(const ProjectionTypeGrp ProjectionTypeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ProjectionType < 0)||(ProjectionType > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ProjectionType",(ProjectionTypeGrp)ProjectionTypeIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ProjectionType = ProjectionTypeIn;
   return(CIGI_SUCCESS);

}


