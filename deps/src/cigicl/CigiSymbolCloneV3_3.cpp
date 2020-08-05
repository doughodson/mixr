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
 *  FILENAME:   CigiSymbolCloneV3_3.cpp
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
 *  04/03/2008 Greg Basler                       CIGI_SYM_1
 *  Initial Release.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_


#include "CigiSymbolCloneV3_3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"



// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolCloneV3_3::CigiSymbolCloneV3_3(void)
{
   PacketID = CIGI_SYMBOL_CLONE_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_CLONE_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;

   SymbolID = 0;
   SourceType = CigiBaseSymbolClone::SymbolSrc;
   SourceID = 0;

}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolCloneV3_3::~CigiSymbolCloneV3_3(void)
{
}



// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolCloneV3_3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiSymbolCloneV3_3 * Data =
      ( CigiSymbolCloneV3_3 *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->SymbolID;

   if(Data->SourceType == CigiBaseSymbolClone::SymbolTemplateSrc)
      *CDta.c++ = 0x01;
   else
      *CDta.c++ = 0;

   *CDta.c++ = 0;

   *CDta.s++ = Data->SourceID;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolCloneV3_3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;
   unsigned char HDta = 0;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {

      SymbolID = *CDta.s++;

      HDta = *CDta.c++;

      CDta.c++;

      SourceID = *CDta.s++;

   }
   else
   {

      CigiSwap2(&SymbolID, CDta.s++);

      HDta = *CDta.c++;

      CDta.c++;

      CigiSwap2(&SourceID, CDta.s++);

   }

   SourceType = (CigiBaseSymbolClone::SymbolSourceGrp)(HDta & 0x01);

   return(PacketSize);

}




