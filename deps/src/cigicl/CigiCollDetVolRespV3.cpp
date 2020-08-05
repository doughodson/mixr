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
 *  FILENAME:   CigiCollDetVolRespV3.cpp
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

#include "CigiCollDetVolRespV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiCollDetVolRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolRespV3::CigiCollDetVolRespV3()
{

   PacketID = CIGI_COLL_DET_VOL_RESP_PACKET_ID_V3;
   PacketSize = CIGI_COLL_DET_VOL_RESP_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   EntityID = 0;
   VolID = 0;
   CollType = NonEntity;
   CollEntityID = 0;
   CollVolID = 0;

}

// ================================================
// ~CigiCollDetVolRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolRespV3::~CigiCollDetVolRespV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolRespV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseCollDetVolResp * Data = ( CigiBaseCollDetVolResp *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->EntityID;
   *CDta.c++ = Data->VolID;
   *CDta.c++ = (Cigi_uint8)(Data->CollType & 0x01);
   *CDta.s++ = Data->CollEntityID;
   *CDta.c++ = Data->CollVolID;
   *CDta.c++ = 0;
   *CDta.s++ = 0;
   *CDta.l++ = 0;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolRespV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      EntityID = *CDta.s++;
      VolID = *CDta.c++;
      CollType = (CollTypeGrp)(*CDta.c++ & 0x01);
      CollEntityID = *CDta.s++;
      CollVolID = *CDta.c++;
   }
   else
   {
      CigiSwap2(&EntityID, CDta.s++);
      VolID = *CDta.c++;
      CollType = (CollTypeGrp)(*CDta.c++ & 0x01);
      CigiSwap2(&CollEntityID, CDta.s++);
      CollVolID = *CDta.c++;
   }

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


