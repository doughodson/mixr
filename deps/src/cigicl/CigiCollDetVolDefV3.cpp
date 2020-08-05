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
 *  FILENAME:   CigiCollDetVolDefV3.cpp
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

#include "CigiCollDetVolDefV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiCollDetVolDefV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolDefV3::CigiCollDetVolDefV3()
{

   PacketID = CIGI_COLL_DET_VOL_DEF_PACKET_ID_V3;
   PacketSize = CIGI_COLL_DET_VOL_DEF_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   EntityID = 0;
   VolID = 0;
   VolEn = false;
   VolType = Sphere;
   Xoff = 0.0;
   Yoff = 0.0;
   Zoff = 0.0;
   HeightOrRadius = 0.0;
   Width = 0.0;
   Depth = 0.0;
   Roll = 0.0;
   Pitch = 0.0;
   Yaw = 0.0;

}

// ================================================
// ~CigiCollDetVolDefV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolDefV3::~CigiCollDetVolDefV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseCollDetVolDef * Data = ( CigiBaseCollDetVolDef *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->EntityID;
   *CDta.c++ = Data->VolID;

   Cigi_uint8 HDta = (Data->VolEn) ? 0x01 : 0;
   HDta |= (Data->VolType << 1) & 0x02;
   *CDta.c++ = HDta;

   *CDta.s++ = 0;

   *CDta.f++ = Data->Xoff;
   *CDta.f++ = Data->Yoff;
   *CDta.f++ = Data->Zoff;
   *CDta.f++ = Data->HeightOrRadius;
   *CDta.f++ = Data->Width;
   *CDta.f++ = Data->Depth;
   *CDta.f++ = Data->Roll;
   *CDta.f++ = Data->Pitch;
   *CDta.f++ = Data->Yaw;

   *CDta.l = 0;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      EntityID = *CDta.s++;
      VolID = *CDta.c++;

      Cigi_uint16 HDta = *CDta.c++;
      VolEn = ((HDta & 0x01) != 0);
      VolType = (VolTypeGrp)((HDta >> 1) & 0x01);

      CDta.s++;

      Xoff = *CDta.f++;
      Yoff = *CDta.f++;
      Zoff = *CDta.f++;
      HeightOrRadius = *CDta.f++;
      Width = *CDta.f++;
      Depth = *CDta.f++;
      Roll = *CDta.f++;
      Pitch = *CDta.f++;
      Yaw = *CDta.f++;
   }
   else
   {
      CigiSwap2(&EntityID, CDta.s++);
      VolID = *CDta.c++;

      Cigi_uint16 HDta = *CDta.c++;
      VolEn = ((HDta & 0x01) != 0);
      VolType = (VolTypeGrp)((HDta >> 1) & 0x01);

      CDta.s++;

      CigiSwap4(&Xoff, CDta.f++);
      CigiSwap4(&Yoff, CDta.f++);
      CigiSwap4(&Zoff, CDta.f++);
      CigiSwap4(&HeightOrRadius, CDta.f++);
      CigiSwap4(&Width, CDta.f++);
      CigiSwap4(&Depth, CDta.f++);
      CigiSwap4(&Roll, CDta.f++);
      CigiSwap4(&Pitch, CDta.f++);
      CigiSwap4(&Yaw, CDta.f++);
   }

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// VolType
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV3::SetVolType(const VolTypeGrp VolTypeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((VolTypeIn < 0)||(VolTypeIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("VolType",(VolTypeGrp)VolTypeIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   VolType = VolTypeIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Roll
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV3::SetRoll(const float RollIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((RollIn < -180.0)||(RollIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Roll",(float)RollIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Roll = RollIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Pitch
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV3::SetPitch(const float PitchIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((PitchIn < -90.0)||(PitchIn > 90.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Pitch",(float)PitchIn,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Pitch = PitchIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Yaw
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV3::SetYaw(const float YawIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((YawIn < 0.0)||(YawIn > 360.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Yaw",(float)YawIn,0.0,360.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Yaw = YawIn;
   return(CIGI_SUCCESS);

}


