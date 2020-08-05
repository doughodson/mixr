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
 *  FILENAME:   CigiPositionRespV3.cpp
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

#include "CigiPositionRespV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiPositionRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiPositionRespV3::CigiPositionRespV3()
{

   PacketID = CIGI_POSITION_RESP_PACKET_ID_V3;
   PacketSize = CIGI_POSITION_RESP_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   ObjectID = 0;
   ArtPartID = 0;
   ObjectClass = Entity;
   CoordSys = Geodetic;
   LatOrXoff = 0.0;
   LonOrYoff = 0.0;
   AltOrZoff = 0.0;
   Roll = 0.0;
   Pitch = 0.0;
   Yaw = 0.0;

}

// ================================================
// ~CigiPositionRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiPositionRespV3::~CigiPositionRespV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiPositionRespV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBasePositionResp * Data = ( CigiBasePositionResp *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->ObjectID;
   *CDta.c++ = Data->ArtPartID;

   Cigi_uint8 HDta = Data->ObjectClass & 0x07;
   HDta |= (Data->CoordSys << 3) & 0x18;
   *CDta.c++ = HDta;

   *CDta.s++ = 0;

   *CDta.d++ = Data->LatOrXoff;
   *CDta.d++ = Data->LonOrYoff;
   *CDta.d++ = Data->AltOrZoff;
   *CDta.f++ = Data->Roll;
   *CDta.f++ = Data->Pitch;
   *CDta.f++ = Data->Yaw;

   *CDta.l++ = 0;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiPositionRespV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      ObjectID = *CDta.s++;
      ArtPartID = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;
      ObjectClass = (ObjectClassGrp)(HDta & 0x07);
      CoordSys = (CoordSysGrp)((HDta >> 3) & 0x03);

      CDta.s++;

      LatOrXoff = *CDta.d++;
      LonOrYoff = *CDta.d++;
      AltOrZoff = *CDta.d++;
      Roll = *CDta.f++;
      Pitch = *CDta.f++;
      Yaw = *CDta.f++;
   }
   else
   {
      CigiSwap2(&ObjectID, CDta.s++);
      ArtPartID = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;
      ObjectClass = (ObjectClassGrp)(HDta & 0x07);
      CoordSys = (CoordSysGrp)((HDta >> 3) & 0x03);

      CDta.s++;

      CigiSwap8(&LatOrXoff, CDta.d++);
      CigiSwap8(&LonOrYoff, CDta.d++);
      CigiSwap8(&AltOrZoff, CDta.d++);
      CigiSwap4(&Roll, CDta.f++);
      CigiSwap4(&Pitch, CDta.f++);
      CigiSwap4(&Yaw, CDta.f++);
   }

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


